#include "collision.h"

float timeToMove(float aPosition, float bPosition, float aVelocity, float bVelocity)
{
    float totalVelocity = aVelocity - bVelocity;
    float dst = bPosition - aPosition;
    return (dst / totalVelocity);
}
glm::vec3 timeToMove3D(glm::vec3 objAPos, glm::vec3 objAVel, glm::vec3 objBPos, glm::vec3 objBVel)
{
    std::vector<float> axisTimes = {};
    
    // if the time is negative then ofc the object will hit each other in the pass
    unsigned int negativeCount = 0;
    unsigned int maxNegative = 3;
    for (int axis = 0; axis < 3; axis++)
    {
        float time = timeToMove(objAPos[axis], objBPos[axis], objAVel[axis], objBVel[axis]);
        
        axisTimes.push_back(time);

        // Objcet will never reach each other
        if ((objAVel[axis] - objBVel[axis]) == 0.0f) continue;

        if (time < 0.0f) negativeCount++;
        else if (time == 0.0f) maxNegative--;
    }

    if (negativeCount == 0 || negativeCount == maxNegative)
    {
        // Object and target will hit each other in the future
        return glm::vec3(axisTimes[0], axisTimes[1], axisTimes[2]);
    }
    // Object and the target will never hit each other
    return glm::vec3(NULL);
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
            side = objPos[axis] - tarBorder[axis];
            if (side > 0)
            {
                objBorder[axis] -= objScale[axis] / 2;
            }
            else if (side < 0)
            {
                objBorder[axis] += objScale[axis] / 2;
            }
        }
        else if (side < 0)
        {
            // Object is on negative side compare to target position
            tarBorder[axis] -= tarScale[axis] / 2;

            // Calculate object border according to if the object is within target border or not
            side = objPos[axis] - tarBorder[axis];
            if (side > 0)
            {
                objBorder[axis] -= objScale[axis] / 2;
            }
            else if (side < 0)
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

    std::vector<std::vector<unsigned int>> pairs;

    for (unsigned int i = 0; i < objects.size(); i++)
    {
        std::shared_ptr<object::objectBaseClass> currentObject = objects.at(i);
        if (!currentObject->canCollide())
        {
            std::cout << "Object skip" << std::endl;
            continue;
        }
        std::vector<unsigned int> currentPair = {i};

        for (unsigned int j = i + 1; j < objects.size(); j++)
        {
            std::shared_ptr<object::objectBaseClass> targetObject = objects.at(j);
            if (!targetObject->canCollide())
            {
                std::cout << "Target skip" << std::endl;
                continue;
            }
            //pairs.push_back(std::vector<unsigned int>({i, j}));
            currentPair.push_back(j);
        }

        if (currentPair.size() >= 1)
        {
            pairs.push_back(currentPair);
        }
    }

    return pairs;
}
glm::vec3 collision::dstBaseCD(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target, float deltaTime)
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

    unsigned int collideAxis = 0;
    glm::vec3 travelTimes(0.0f, 0.0f, 0.0f);
    for (int axis = 0; axis < 3; axis++)
    {
        // Need more improvement on if else logic

        if (objVel[axis] == tarVel[axis])
        {
            if ((tarBorder[axis] - objBorder[axis]) == 0.0f)
            {
                collideAxis++;
            }
            continue;
        }

        // If the travelTime is 0 then both border of the object and the target is on the same spot
        float travelTime = timeToMove(objBorder[axis], tarBorder[axis], objVel[axis], tarVel[axis]);
        if ((deltaTime > 0.0f && travelTime <= deltaTime && travelTime > 0.0f) || (deltaTime < 0.0f && travelTime >= deltaTime && travelTime < 0.0f) || travelTime == 0.0f)
        {
            collideAxis++;
            travelTimes[axis] = travelTime;
        }
    }

    if (collideAxis == 3)
    {
        return travelTimes;
    }
    
    // Object did collide with target
    return glm::vec3(0.0f);
}
void collision::collsionResolver(std::shared_ptr<object::objectBaseClass> currentObject, std::shared_ptr<object::objectBaseClass> target, glm::vec3 travelTime)
{
    std::cout << "      -  Resolving" << std::endl;
    std::cout << "         -  Old object velocity: " << currentObject->getVelocity().x << ", " << currentObject->getVelocity().y << ", " << currentObject->getVelocity().z << std::endl;
    std::cout << "         -  Old target velocity: " << target->getVelocity().x << ", " << target->getVelocity().y << ", " << target->getVelocity().z << std::endl;

    // Move to collision site
    currentObject->move(fundamental::calculateDstVecT(currentObject->getVelocity(), travelTime));
    target->move(fundamental::calculateDstVecT(target->getVelocity(), travelTime));

    // Update new velocity
    glm::vec3 objNewVel = momentum::elasticCollision(currentObject, target);
    glm::vec3 tarNewVel = momentum::elasticCollision(target, currentObject);

    currentObject->changeVelocity(objNewVel - currentObject->getVelocity());
    target->changeVelocity(tarNewVel - target->getVelocity());

    std::cout << "         -  New object velocity: " << currentObject->getVelocity().x << ", " << currentObject->getVelocity().y << ", " << currentObject->getVelocity().z << std::endl;
    std::cout << "         -  New target velocity: " << target->getVelocity().x << ", " << target->getVelocity().y << ", " << target->getVelocity().z << std::endl;
}
