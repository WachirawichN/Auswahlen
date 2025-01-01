#include "collision.h"

enum side {
    LEFT = -1,
    MIDDLE = 0,
    RIGHT = 1
};

int convertNegative(bool input)
{
    return (input == 0) ? -1 : 1;
}

float timeToMove(float aPosition, float aVelocity, float bPosition, float bVelocity)
{
    float totalVelocity = aVelocity - bVelocity;
    float dst = bPosition - aPosition;
    return (dst / totalVelocity);
}
std::vector<float> unbalanceTimeToMove(float aPosition, float aVelocity, float aRemainTime, float bPosition, float bVelocity, float bRemainTime)
{
    unsigned int lowestTime = (aRemainTime < bRemainTime) ? 0 : 1;
    return std::vector<float>({0.0f});
}

bool isInside(std::vector<float> aBorders, std::vector<float> bBorders)
{
    bool positiveSide = (bBorders[2] < aBorders[2]) && (bBorders[0] < aBorders[2]);
    bool negativeSide = (bBorders[2] > aBorders[0]) && (bBorders[0] > aBorders[0]);

    return (positiveSide && negativeSide);
}
bool isCross(std::vector<float> aBorders, std::vector<float> bBorders)
{
    bool positiveSide = ((bBorders[0] < aBorders[2]) && !(bBorders[2] < aBorders[2])) || (!(bBorders[0] < aBorders[2]) && (bBorders[2] < aBorders[2]));
    bool negativeSide = ((bBorders[2] < aBorders[0]) && !(bBorders[0] < aBorders[0])) || (!(bBorders[2] < aBorders[0]) && (bBorders[0] < aBorders[0]));

    return (positiveSide || negativeSide);
}
side checkSide(float aPos, float bPos)
{
    float delta = aPos - bPos;
    return (delta == 0.0f) ? MIDDLE : (delta > 0.0f) ? LEFT : RIGHT;
}
side checkDirection(float input)
{
    return (input == 0.0f) ? MIDDLE : (input > 0.0f) ? RIGHT : LEFT;
}

std::vector<float> extendBorder(float objPos, float objScale)
{
    // Return format: {-Axis, Origin, +Axis}
    std::vector<float> borders;
    borders.push_back(objPos - objScale / 2.0f);
    borders.push_back(objPos);
    borders.push_back(objPos + objScale / 2.0f);

    return borders;
}
std::vector<int> selectBorder(float objPos, float objVel, float tarPos, float tarVel, bool tarInObj, bool objInTar, float timeMultiplier)
{
    // Function will return -1, 1 so its need to be +1 before use
    int objToTarSide = checkSide(objPos, tarPos);
    bool isObjFaster = abs(objVel) > abs(tarVel);
    int objVelDirection = checkDirection(objVel * timeMultiplier);
    int tarVelDirection = checkDirection(tarVel * timeMultiplier);

    // Hollow body
    if (objInTar)
    {
        if (objVelDirection != tarVelDirection)
        {
            return std::vector<int>({objVelDirection, objVelDirection});
        }
        return std::vector<int>({convertNegative(isObjFaster) * objVelDirection, convertNegative(isObjFaster) * objVelDirection});
    }
    else if (tarInObj)
    {
        if (objVelDirection != tarVelDirection)
        {
            return std::vector<int>({tarVelDirection, tarVelDirection});
        }
        return std::vector<int>({convertNegative(isObjFaster) * tarVelDirection, convertNegative(isObjFaster) * tarVelDirection});
    }

    // Solid body
    return std::vector<int>({objToTarSide, objToTarSide * -1});
}

std::vector<std::vector<unsigned int>> collision::collisionPairing(std::vector<std::shared_ptr<object::objectBaseClass>> objects)
{
    // The algorithm should sort the pairing base on the distance of each pair

    // Object at index 0 is the current object then after that is all the target object
    std::vector<std::vector<unsigned int>> pairs;

    for (unsigned int i = 0; i < objects.size(); i++)
    {
        std::shared_ptr<object::objectBaseClass> currentObject = objects.at(i);
        if (!currentObject->canCollide()) continue;
        std::vector<unsigned int> currentPair = {i};

        for (unsigned int j = i + 1; j < objects.size(); j++)
        {
            std::shared_ptr<object::objectBaseClass> targetObject = objects.at(j);
            if (!targetObject->canCollide()) continue;
            currentPair.push_back(j);
        }

        if (currentPair.size() >= 1)
        {
            pairs.push_back(currentPair);
        }
    }

    return pairs;
}
std::vector<collision::collisionType> collision::CCD(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target, float deltaTime)
{
    std::vector<collision::collisionType> collisionResults;
    int timeMultiplier = (deltaTime > 0.0f) ? 1 : -1;

    glm::vec3 objPos = object->getPosition();
    glm::vec3 objScale = object->getScale();
    glm::vec3 objVel = object->getVelocity();
    float objectDeltaTime = object->getCollisionTime();

    glm::vec3 tarPos = target->getPosition();
    glm::vec3 tarScale = target->getScale();
    glm::vec3 tarVel = target->getVelocity();
    float targetDeltaTime = object->getCollisionTime();

    std::cout << "   -  Selected border:" << std::endl;
    for (int axis = 0; axis < 3; axis++)
    {
        // Calculate the border of both object
        std::vector<float> objBorders = extendBorder(objPos[axis], objScale[axis]);
        std::vector<float> tarBorders = extendBorder(tarPos[axis], tarScale[axis]);

        // Check if the object is inside each other or not
        bool tarInObj = isInside(objBorders, tarBorders);
        bool objInTar = isInside(tarBorders, objBorders);


        // Choose object border
        std::vector<int> selectedBorder = selectBorder(objPos[axis], objVel[axis], tarPos[axis], tarVel[axis], tarInObj, objInTar, timeMultiplier);
        int objBorderIndex = selectedBorder[0] + 1;
        int tarBorderIndex = selectedBorder[1] + 1;

        float objBorder = objBorders[objBorderIndex];
        float tarBorder = tarBorders[tarBorderIndex];
        std::cout << "      -  Axis: " << axis << ", object's: " << objBorderIndex << ", target's: " << tarBorderIndex << std::endl;

        // Check for NEWLY collision type
        float travelTime = timeToMove(objBorder, objVel[axis], tarBorder, tarVel[axis]);
        if ((deltaTime > 0.0f && deltaTime > travelTime) || (deltaTime < 0.0f && deltaTime < travelTime) || (travelTime == 0.0f))
        {
            if (checkDirection(deltaTime) == checkDirection(travelTime))
            {
                collisionResults.push_back(collision::collisionType::NEWLY);
                continue;
            }
        }

        // Check for CROSS and INSIDE colision type
        if (objInTar || tarInObj)
        {
            collisionResults.push_back(collision::collisionType::INSIDE);
            continue;
        }
        else if (isCross(objBorders, tarBorders))
        {
            collisionResults.push_back(collision::collisionType::CROSS);
            continue;
        }

        // No collision has been occure
        collisionResults.push_back(collision::collisionType::NO);
    }

    return collisionResults;
}
std::vector<float> collision::collisionResolver(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target, float deltaTime, std::vector<unsigned int> newlyAxis)
{
    // Working instruction:
    // -  Step 1:
    //    -  Find the time that two object need to use to travel to collision site in all axis that is NEWLY collide axis, which is called travelTimes
    // -  Step 2:
    //    -  Sort the travelTimes to find which NEWLY collide axis took the most time to travel to collidision site, the axis that took the most time will be count as the NEWLY collide axis
    //       and the other will now be count as ALREADY collide axis, but if there are more than one axis that took the same amount of time then all of the axis that took the same amount
    //       of time will still be count as NEWLY collide axis
    // -  Step 3:
    //    -  Stepping physic calculation using the travel time that is the most out of all NEWLY collide axis, the other collide axis that took less time will be count as ALREADY collide axis.
    //       If there is other NEWLY collide axis that took the same amount of time then those two or three axis will be still be NEWLY collide axis
    // -  Step 4:
    //    -  Calculate new momentum of two object in the axis that is NEWLY collide axis

    int timeMultiplier = (deltaTime > 0.0f) ? 1 : -1;

    glm::vec3 objPos = object->getPosition();
    glm::vec3 objScale = object->getScale();
    glm::vec3 objVel = object->getVelocity();

    glm::vec3 tarPos = target->getPosition();
    glm::vec3 tarScale = target->getScale();
    glm::vec3 tarVel = target->getVelocity();

    // Step 1.
    std::vector<float> travelTimes;
    for (unsigned int axis : newlyAxis)
    {
        // Calculate the border of both object
        std::vector<float> objBorders = extendBorder(objPos[axis], objScale[axis]);
        std::vector<float> tarBorders = extendBorder(tarPos[axis], tarScale[axis]);

        // Check if the object is inside each other or not
        bool tarInObj = isInside(objBorders, tarBorders);
        bool objInTar = isInside(tarBorders, objBorders);

        // Choose object border
        std::vector<int> selectedBorder = selectBorder(objPos[axis], objVel[axis], tarPos[axis], tarVel[axis], tarInObj, objInTar, timeMultiplier);
        int objMultiplier = selectedBorder[0] + 1;
        int tarMultiplier = selectedBorder[1] + 1;

        float objBorder = objBorders[objMultiplier];
        float tarBorder = tarBorders[tarMultiplier];

        float travelTime = timeToMove(objBorder, objVel[axis], tarBorder, tarVel[axis]);
        travelTimes.push_back(travelTime);
    }

    // Step 2.
    float highestTime = *std::max_element(travelTimes.begin(), travelTimes.end());
    //std::cout << "      -  Highest time: " << highestTime << std::endl;

    for (unsigned int counted = 0; counted < newlyAxis.size(); counted++)
    {
        std::cout << "         -  Axis: " << newlyAxis[counted] << ", Usage time: " << travelTimes[counted] << std::endl;
    }

    std::vector<unsigned int> newlyCollideAxis;
    for (unsigned int counted = 0; counted < newlyAxis.size(); counted++)
    {
        if (travelTimes[counted] == highestTime)
        {
            newlyCollideAxis.push_back(newlyAxis[counted]);
            std::cout << "      -  Push axis: " << newlyAxis[counted] << std::endl;
        }
        else
        {
            //std::cout << "      -  Ain't pushing axis: " << newlyAxis[counted] << ", highest: " << highestTime << ", usage: " << travelTimes[counted] << ", delta" << highestTime - travelTimes[counted] << std::endl;
        }
    }
    //std::cout << "      -  Newly collided vector size: " << newlyCollideAxis.size() << std::endl;

    // Step 3.
    object->move(fundamental::calculateDst(objVel, highestTime));
    target->move(fundamental::calculateDst(tarVel, highestTime));

    // Step 4.
    for (unsigned int axis : newlyCollideAxis)
    {
        float objNewVel = momentum::elasticCollision1D(object, target, axis);
        float tarNewVel = momentum::elasticCollision1D(target, object, axis);

        glm::vec3 objDeltaVel(0.0f);
        glm::vec3 tarDeltaVel(0.0f);

        objDeltaVel[axis] = objNewVel - objVel[axis];
        tarDeltaVel[axis] = tarNewVel - tarVel[axis];

        object->changeVelocity(objDeltaVel);
        target->changeVelocity(tarDeltaVel);

        std::cout << "      -  Resolved, Axis: " << axis << std::endl;
        std::cout << "         -  Object new axis vel: " << objNewVel << ", og axis vel: " << objVel[axis] << std::endl;
        std::cout << "         -  Target new axis vel: " << tarNewVel << ", og axis vel: " << tarVel[axis] << std::endl;
    }

    // Return the time that has been use
    return std::vector<float>({highestTime, highestTime});
}
