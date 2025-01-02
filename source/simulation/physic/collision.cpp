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
    return (totalVelocity != 0.0f) ? (dst / totalVelocity) : 999999.99f;
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
        // Opposite direction
        if (objVelDirection != tarVelDirection)
        {
            if (objVelDirection == MIDDLE)
            {
                std::cout << "Obj velocity is middle" << std::endl;
                return std::vector<int>({tarVelDirection, tarVelDirection});
            }
            std::cout << "Obj velocity is not middle" << std::endl;
            return std::vector<int>({objVelDirection, objVelDirection});
        }

        // Same direction
        if (objVelDirection != MIDDLE)
        {
            std::cout << "None is middle: " << convertNegative(isObjFaster) << ", " << tarVelDirection << std::endl;
            return std::vector<int>({convertNegative(isObjFaster) * objVelDirection, convertNegative(isObjFaster) * objVelDirection});
        }
    }
    else if (tarInObj)
    {
        // Opposite direction
        if (objVelDirection != tarVelDirection)
        {
            if (tarVelDirection == MIDDLE)
            {
                std::cout << "Tar velocity is middle" << std::endl;
                return std::vector<int>({objVelDirection, objVelDirection});
            }
            std::cout << "Tar velocity is not middle" << std::endl;
            return std::vector<int>({tarVelDirection, tarVelDirection});
        }

        // Same direction
        if (tarVelDirection != MIDDLE)
        {
            std::cout << "None is middle: " << convertNegative(isObjFaster) << ", " << tarVelDirection << std::endl;
            return std::vector<int>({convertNegative(!isObjFaster) * tarVelDirection, convertNegative(!isObjFaster) * tarVelDirection});
        }
    }

    // Solid body
    std::cout << "Solid body" << std::endl;
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
std::vector<collision::collisionType> collision::CCD(std::shared_ptr<object::objectBaseClass> obj, std::shared_ptr<object::objectBaseClass> tar, float deltaTime)
{
    std::vector<collision::collisionType> collisionResults;
    int timeMultiplier = (deltaTime > 0.0f) ? 1 : -1;

    glm::vec3 objPos = obj->getPosition();
    glm::vec3 objScale = obj->getScale();
    glm::vec3 objVel = obj->getVelocity();
    float objectDeltaTime = obj->getCollisionTime();

    glm::vec3 tarPos = tar->getPosition();
    glm::vec3 tarScale = tar->getScale();
    glm::vec3 tarVel = tar->getVelocity();
    float targetDeltaTime = obj->getCollisionTime();

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
        if ((abs(deltaTime) > 0.0f && abs(deltaTime) > abs(travelTime)) || travelTime == 0.0f)
        {
            if (checkDirection(deltaTime) == checkDirection(travelTime))
            {
                collisionResults.push_back(collision::collisionType::NEWLY);
                std::cout << "         -  Distance: " << tarBorder - objBorder << ", Velocity: " << objVel[axis] - tarVel[axis] << std::endl;
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
float collision::collisionResolver(std::shared_ptr<object::objectBaseClass> obj, std::shared_ptr<object::objectBaseClass> tar, float deltaTime, std::vector<unsigned int> newlyAxis)
{
    // Working instruction:
    // -  Step 1:
    //    -  Find the time that two object need to use to travel to collision site in all axis that is NEWLY collide axis.
    // -  Step 2:
    //    -  Put those travel times through pythagoras to get final time to move before collision will be occured.
    // -  Step 3:
    //    -  Move those object to collision site.
    // -  Step 4:
    //    -  Update object and target velocity that is NEWLY collide axis.

    int timeMultiplier = (deltaTime > 0.0f) ? 1 : -1;

    glm::vec3 objPos = obj->getPosition();
    glm::vec3 objScale = obj->getScale();
    glm::vec3 objVel = obj->getVelocity();

    glm::vec3 tarPos = tar->getPosition();
    glm::vec3 tarScale = tar->getScale();
    glm::vec3 tarVel = tar->getVelocity();

    // Step 1.
    glm::vec3 travelTimes(0.0f);
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
        travelTimes[axis] = travelTime;
    }

    // Step 2.
    float pythaTime = mathExt::pythagoras(travelTimes);

    // Step 3.
    obj->move(fundamental::calculateDst(objVel, pythaTime));
    tar->move(fundamental::calculateDst(tarVel, pythaTime));

    // Step 4.
    glm::vec3 objDeltaVel(0.0f);
    glm::vec3 tarDeltaVel(0.0f);
    std::cout << "      -  Resolving collision" << std::endl;
    for (unsigned int axis : newlyAxis)
    {
        float objNewVel = momentum::elasticCollision1D(obj, tar, axis);
        float tarNewVel = momentum::elasticCollision1D(tar, obj, axis);

        tarDeltaVel[axis] = tarNewVel - tarVel[axis];
        objDeltaVel[axis] = objNewVel - objVel[axis];

        std::cout << "         -  Axis: " << axis << std::endl;
        std::cout << "            -  Object original axis vel: " << objVel[axis] << ", new axis vel: " << objNewVel << std::endl;
        std::cout << "            -  Target original axis vel: " << tarVel[axis] << ", new axis vel: " << tarNewVel << std::endl;
        std::cout << "            -  Object original axis pos: " << objPos[axis] << ", new axis pos: " << obj->getPosition()[axis] << std::endl;
        std::cout << "            -  Target original axis pos: " << tarPos[axis] << ", new axis pos: " << tar->getPosition()[axis] << std::endl;
    }
    obj->changeVelocity(objDeltaVel);
    tar->changeVelocity(tarDeltaVel);

    return pythaTime;
}
