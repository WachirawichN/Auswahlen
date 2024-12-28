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

bool isInside(float aPos, float aScale, float bPos, float bScale)
{
    bool positiveSide = ((bPos + bScale / 2.0f) < (aPos + aScale / 2.0f)) && ((bPos - bScale / 2.0f) < (aPos + aScale / 2.0f));
    bool negativeSide = ((bPos + bScale / 2.0f) > (aPos - aScale / 2.0f)) && ((bPos - bScale / 2.0f) > (aPos - aScale / 2.0f));

    return (positiveSide && negativeSide);
}
bool isCross(float aPos, float aScale, float bPos, float bScale)
{
    bool positiveSide = (bPos - bScale / 2.0f) < (aPos + aScale / 2.0f);
    bool negativeSide = (bPos + bScale / 2.0f) < (aPos - aScale / 2.0f);

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
        //std::cout << "Hollow body" << std::endl;
        if (objVelDirection != tarVelDirection)
        {
            //std::cout << "Same direction" << std::endl;
            return std::vector<int>({objVelDirection, objVelDirection});
        }
        return std::vector<int>({convertNegative(isObjFaster) * objVelDirection, convertNegative(isObjFaster) * objVelDirection});
    }
    else if (tarInObj)
    {
        //std::cout << "Hollow body" << std::endl;
        if (objVelDirection != tarVelDirection)
        {
            //std::cout << "Same direction" << std::endl;
            return std::vector<int>({tarVelDirection, tarVelDirection});
        }
        return std::vector<int>({convertNegative(isObjFaster) * tarVelDirection, convertNegative(isObjFaster) * tarVelDirection});
    }

    // Solid body
    //std::cout << "Solid body" << std::endl;
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
std::vector<collision::collisionType> collision::dstBaseCD(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target, float deltaTime)
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


    std::cout << "   -  Position / Border: " << std::endl;
    for (int axis = 0; axis < 3; axis++)
    {
        // Check if the object is inside each other or not
        // Inside mean the center of the object is within the border of the other object
        bool tarInObj = isInside(objPos[axis], objScale[axis], tarPos[axis], tarScale[axis]);
        bool objInTar = isInside(tarPos[axis], tarScale[axis], objPos[axis], objScale[axis]);


        // Calculate the border of both object
        std::vector<float> objBorders = extendBorder(objPos[axis], objScale[axis]);
        std::vector<float> tarBorders = extendBorder(tarPos[axis], tarScale[axis]);

        // Choose object border
        std::vector<int> selectedBorder = selectBorder(objPos[axis], objVel[axis], tarPos[axis], tarVel[axis], tarInObj, objInTar, timeMultiplier);
        int objMultiplier = selectedBorder[0] + 1;
        int tarMultiplier = selectedBorder[1] + 1;

        float objBorder = objBorders[objMultiplier];
        float tarBorder = tarBorders[tarMultiplier];

        std::cout << "      -  Axis: " << axis << std::endl;
        std::cout << "         -  Object multiplier: " << objMultiplier - 1 << std::endl;
        std::cout << "         -  Target multiplier: " << tarMultiplier - 1 << std::endl;
        std::cout << "         -  Target inside object: " << tarInObj << std::endl;
        std::cout << "         -  Object inside Target: " << objInTar << std::endl;


        // Check for collision
        // Check for NEWLY collision type
        // Calculate time that the object and the target will use to collide each other
        float travelTime = timeToMove(objBorder, objVel[axis], tarBorder, tarVel[axis]);
        if ((deltaTime > 0.0f && deltaTime > travelTime) || (deltaTime < 0.0f && deltaTime < travelTime) || (travelTime == 0.0f))
        {
            std::cout << "Delta time: " << deltaTime << ", Travel time: " << travelTime << std::endl;
            if (checkDirection(deltaTime) == checkDirection(travelTime))
            {
                collisionResults.push_back(collision::collisionType::NEWLY);
                continue;
            }
        }

        // Check for CROSS colision type
        if (objInTar || tarInObj)
        {
            collisionResults.push_back(collision::collisionType::INSIDE);
            continue;
        }
        else if (isCross(objPos[axis], objScale[axis], tarPos[axis], tarScale[axis]))
        {
            collisionResults.push_back(collision::collisionType::CROSS);
            continue;
        }


        // No collision has been occure
        collisionResults.push_back(collision::collisionType::NO);
    }

    return collisionResults;
}
std::vector<float> collision::collsionResolver(std::shared_ptr<object::objectBaseClass> object, float objectDeltaTime, std::shared_ptr<object::objectBaseClass> target, float targetDeltaTime, std::vector<unsigned int> axis)
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

    glm::vec3 objPos = object->getPosition();
    glm::vec3 objScale = object->getScale();
    glm::vec3 objVel = object->getVelocity();

    glm::vec3 tarPos = target->getPosition();
    glm::vec3 tarScale = target->getScale();
    glm::vec3 tarVel = target->getVelocity();

    // Step 1.
    std::vector<float> travelTimes;
    for (unsigned int i : axis)
    {
        side objToTarSide = checkSide(objPos[i], tarPos[i]);
        float objBorder = objPos[i] + objScale[i] / 2.0f * (objToTarSide * -2 + 1);
        float tarBorder = tarPos[i] + tarScale[i] / 2.0f * (objToTarSide * 2 - 1);
        float travelTime = mathExt::roundToDec(timeToMove(objBorder, objVel[i], tarBorder, tarVel[i]), 6);
        travelTimes.push_back(travelTime);
    }

    // Step 2.
    float highestTime = *std::max_element(travelTimes.begin(), travelTimes.end());
    std::vector<unsigned int> newlyCollideAxis;
    for (unsigned int i : axis)
    {
        if (travelTimes[i] == highestTime) newlyCollideAxis.push_back(i);
    }

    // Step 3.
    object->move(fundamental::calculateDst(objVel, highestTime));
    target->move(fundamental::calculateDst(tarVel, highestTime));

    // Step 4.
    for (unsigned int i : newlyCollideAxis)
    {
        object->changeVelocity(momentum::elasticCollision1D(object, target, i) - objVel);
        target->changeVelocity(momentum::elasticCollision1D(target, object, i) - tarVel);
    }

    // Return the time that has been use
    return std::vector<float>({highestTime, highestTime});
}
