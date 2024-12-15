#include "collision.h"

enum side {
    LEFT,
    RIGHT
};

float timeToMove(float aPosition, float aVelocity, float bPosition, float bVelocity)
{
    float totalVelocity = aVelocity - bVelocity;
    float dst = bPosition - aPosition;
    return (dst / totalVelocity);
}
bool isInside(float aPos, float aScale, float bPos)
{
    if ((bPos < (aPos + aScale / 2.0f)) && (bPos > (aPos - aScale / 2.0f))) return true;
    return false;
}
bool isSameDirection(float inputA, float inputB)
{
    if ((inputA < 0.0f && inputB < 0.0f) || (inputA > 0.0f && inputB > 0.0f) || (inputA == 0.0f && inputB == 0.0f)) return true;
    return false;
}
side checkSide(float aPos, float bPos)
{
    float side = aPos - bPos;
    if (side < 0.0f) return RIGHT;
    return LEFT;
}
std::vector<float> extendBorder(float objPos, float objScale)
{
    // Return format: {-Axis, +Axis}
    std::vector<float> borders;
    borders.push_back(objPos - objScale / 2.0f);
    borders.push_back(objPos + objScale / 2.0f);

    return borders;
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

    glm::vec3 objPos = object->getPosition();
    glm::vec3 objScale = object->getScale();
    glm::vec3 objVel = object->getVelocity();

    glm::vec3 tarPos = target->getPosition();
    glm::vec3 tarScale = target->getScale();
    glm::vec3 tarVel = target->getVelocity();

    for (int axis = 0; axis < 3; axis++)
    {
        // Check if the object is inside each other or not
        if (isInside(objPos[axis], objScale[axis], tarPos[axis]) || isInside(tarPos[axis], tarScale[axis], objPos[axis]))
        {
            collisionResults.push_back(collision::collisionType::INSIDE);
            continue;
        }


        //Check if the collision of this axis is ALREADY collide or not
        std::vector<float> objBorders = extendBorder(objPos[axis], objScale[axis]);

        // Calculate the border of the target that need to be compare to the object's borders
        side objToTarSide = checkSide(objPos[axis], tarPos[axis]);
        float targetBorders = tarPos[axis] + tarScale[axis] / 2.0f * (objToTarSide * 2 - 1);

        if (checkSide(objBorders[0], targetBorders) != checkSide(objBorders[1], targetBorders))
        {
            collisionResults.push_back(collision::collisionType::ALREADY);
            continue;
        }


        // Check if the collision of this axis is NEWLY collide or not, if not the return vector of 0.0f
        // Choose object's border to compare its before and after
        float objBorder = objBorders[objToTarSide];
        float objBorderAfter = objBorder + objVel[axis] * deltaTime;
        float tarBorderAfter = targetBorders + tarVel[axis] * deltaTime;
        if (checkSide(objBorder, targetBorders) != checkSide(objBorderAfter, tarBorderAfter))
        {
            collisionResults.push_back(collision::collisionType::NEWLY);
            continue;
        }

        // No collision has been occured in this axis
        collisionResults.push_back(collision::collisionType::NO);
    }

    return collisionResults;
}
float collision::collsionResolver(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target, float deltaTime, std::vector<unsigned int> axis)
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
    return highestTime;
}
