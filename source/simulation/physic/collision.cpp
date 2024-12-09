#include "collision.h"

float timeToMove(float aPosition, float bPosition, float aVelocity, float bVelocity)
{
    float totalVelocity = aVelocity - bVelocity;
    float dst = bPosition - aPosition;
    return (dst / totalVelocity);
}
std::vector<glm::vec3> borderPos(glm::vec3 objPos, glm::vec3 objScale, glm::vec3 tarPos, glm::vec3 tarScale)
{
    std::vector<glm::vec3> borders;

    glm::vec3 objBorder = objPos;
    glm::vec3 tarBorder = tarPos;

    for (int axis = 0; axis < 3; axis++)
    {
        float side = objPos[axis] - tarPos[axis];
        if (side > 0)
        {
            // Object is on positive side compare to target position
            tarBorder[axis] += tarScale[axis] / 2;

            // Calculate object border according to if the object is within target border or not
            float subSide = objPos[axis] - tarBorder[axis];
            if (subSide > 0)
            {
                objBorder[axis] -= objScale[axis] / 2;
            }
            else if (subSide < 0)
            {
                objBorder[axis] += objScale[axis] / 2;
            }
        }
        else if (side < 0)
        {
            // Object is on negative side compare to target position
            tarBorder[axis] -= tarScale[axis] / 2;

            // Calculate object border according to if the object is within target border or not
            float subSide = objPos[axis] - tarBorder[axis];
            if (subSide > 0)
            {
                objBorder[axis] -= objScale[axis] / 2;
            }
            else if (subSide < 0)
            {
                objBorder[axis] += objScale[axis] / 2;
            }
        }
        
    }

    borders.push_back(objBorder);
    borders.push_back(tarBorder);
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
glm::vec3 collision::dstBaseCD(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target, glm::vec3 deltaTime)
{
    glm::vec3 objPos = object->getPosition();
    glm::vec3 objScale = object->getScale();
    glm::vec3 objVel = object->getVelocity();

    glm::vec3 tarPos = target->getPosition();
    glm::vec3 tarScale = target->getScale();
    glm::vec3 tarVel = target->getVelocity();

    // Stretch out border of object and target according to what side of object is correlate to target on all axis
   std::vector<glm::vec3> borders = borderPos(objPos, objScale, tarPos, tarScale);
   glm::vec3 objBorder = borders[0];
   glm::vec3 tarBorder = borders[1];

   std::cout << "   -  Current object border: " << objBorder.x << ", " << objBorder.y << ", " << objBorder.z << std::endl;
   std::cout << "   -  Target object border: " << tarBorder.x << ", " << tarBorder.y << ", " << tarBorder.z << std::endl;

    // Actual collision detection on each axis
    unsigned int collideAxis = 0;
    glm::vec3 travelTimes(0.0f, 0.0f, 0.0f);
    for (int axis = 0; axis < 3; axis++)
    {
        // Need more improvement on if else logic

        if (objVel[axis] == tarVel[axis])
        {
            if (tarBorder[axis] == objBorder[axis])
            {
                collideAxis++;
            }
            continue;
        }

        // If the travelTime is 0 then both border of the object and the target is on the same spot
        float travelTime = mathExt::roundToDecimal(timeToMove(objBorder[axis], tarBorder[axis], objVel[axis], tarVel[axis]), 5);
        if ((deltaTime[axis] > 0.0f && travelTime <= deltaTime[axis] && travelTime > 0.0f) || (deltaTime[axis] < 0.0f && travelTime >= deltaTime[axis] && travelTime < 0.0f) || travelTime == 0.0f)
        {
            collideAxis++;
            travelTimes[axis] = travelTime;
        }
    }

    // Collision have been detected
    if (collideAxis == 3)
    {
        return travelTimes;
    }
    
    // Object did collide with target
    return glm::vec3(0.0f);
}
void collision::collsionResolver(std::shared_ptr<object::objectBaseClass> currentObject, std::shared_ptr<object::objectBaseClass> target, glm::vec3 travelTime)
{
    // Move to collision site
    currentObject->move(fundamental::calculateDstVecT(currentObject->getVelocity(), travelTime));
    target->move(fundamental::calculateDstVecT(target->getVelocity(), travelTime));

    // Update new velocity
    glm::vec3 objNewVel = momentum::elasticCollision(currentObject, target);
    glm::vec3 tarNewVel = momentum::elasticCollision(target, currentObject);
    currentObject->changeVelocity(objNewVel - currentObject->getVelocity());
    target->changeVelocity(tarNewVel - target->getVelocity());
}
