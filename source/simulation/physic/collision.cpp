#include "collision.h"

#include "../object/sphere.h"
#include <iostream>

float pythagoras(glm::vec3 dstVector)
{
    return sqrt(pow(dstVector.x, 2) + pow(dstVector.y, 2) + pow(dstVector.z, 2));
}
float timeToMove(float aPosition, float bPosition, float aVelocity, float bVelocity)
{
    float totalVelocity = bVelocity - aVelocity;
    return (totalVelocity == 0.0f) ? 0.0f : ((aPosition - bPosition) / totalVelocity);
}
float timeToMove3D(glm::vec3 objAPos, glm::vec3 objAVel, glm::vec3 objBPos, glm::vec3 objBVel)
{
    std::vector<float> axisTimes = {};
    int negativeCount = 0;
    std::cout << "      -  Distance and velocity:" << std::endl;
    for (int axis = 0; axis < 3; axis++)
    {
        std::cout << "           -  Distance: " << objBPos[axis] - objAPos[axis] << ", Total velocity: " << objBVel[axis] - objAVel[axis] << std::endl;

        // If the time is below zero will mean the object and the target will hit each other in the past in that axis
        float time = timeToMove(objAPos[axis], objBPos[axis], objAVel[axis], objBVel[axis]);
        if (time < 0.0f) negativeCount++;
        
        axisTimes.push_back(time);
    }

    std::cout << "      -  Negative count: " << negativeCount << std::endl;
    std::cout << "           -  axisTime: " << axisTimes[0] << ", " << axisTimes[1] << ", " << axisTimes[2] << std::endl;
    if (negativeCount == 0)
    {
        // Object and target will hit each other in the future
        return pythagoras(glm::vec3(axisTimes[0], axisTimes[1], axisTimes[2]));
    }
    else if (negativeCount == 3)
    {   
        // Object and target hit each other in the past
        return -pythagoras(glm::vec3(axisTimes[0], axisTimes[1], axisTimes[2]));
    }
    // Object and the target will never hit each other
    return 0.0f;
}

float collision::dstBaseCD(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target, float deltaTime)
{
    glm::vec3 objPos = object->getPosition();
    glm::vec3 objScale = object->getScale();
    glm::vec3 objVel = object->getVelocity();

    glm::vec3 tarPos = target->getPosition();
    glm::vec3 tarScale = target->getScale();
    glm::vec3 tarVel = target->getVelocity();

    std::cout << "      -  Origin position: " << std::endl;
    std::cout << "           -  Object position: " << objPos.x << ", " << objPos.y << ", " << objPos.z << ", target position: " << tarPos.x << ", " << tarPos.y << ", " << tarPos.z << std::endl;

    // Stretch out border of object and target according to whih side of object is correlate to target on all axis
    for (int axis = 0; axis < 3; axis++)
    {
        float side = objPos[axis] - tarPos[axis];
        if (side > 0)
        {
            // Object is on positive side compare to target position
            objPos[axis] -= objScale[axis] / 2;
            tarPos[axis] += tarScale[axis] / 2;
        }
        else if (side < 0)
        {
            // Object is on negative side compare to target position
            objPos[axis] += objScale[axis] / 2;
            tarPos[axis] -= tarScale[axis] / 2;
        }
        
    }

    float travelTime = timeToMove3D(objPos, objVel, tarPos, tarVel);

    // Object did collide with target
    if ((deltaTime > 0.0f && travelTime < deltaTime && travelTime > 0.0f) || (deltaTime < 0.0f && travelTime > deltaTime && travelTime < 0.0f))
    {
        std::cout << "      -  Got collided, Time took: " << std::fixed << std::setprecision(5) << travelTime << ", Delta time: " << deltaTime << std::endl;
        std::cout << "      -  Before:" << std::endl;
        std::cout << "           -  Object border position: " << objPos.x << ", " << objPos.y << ", " << objPos.z << ", target border position: " << tarPos.x << ", " << tarPos.y << ", " << tarPos.z << std::endl;
        std::cout << "           -  Object velocity: " << objVel.x << ", " << objVel.y << ", " << objVel.z << ", target velocity: " << tarVel.x << ", " << tarVel.y << ", " << tarVel.z << std::endl;

        // Move both object to the position that it only contact each other
        object->move(fundamental::calculateDistance(objVel, travelTime));
        target->move(fundamental::calculateDistance(tarVel, travelTime));

        glm::vec3 objNewVel = momentum::elasticCollision(object, target);
        glm::vec3 tarNewVel = momentum::elasticCollision(target, object);
        object->changeVelocity(objNewVel - objVel);
        target->changeVelocity(tarNewVel - tarVel);

        //object->move(fundamental::calculateDistance(objNewVel, deltaTime - travelTime));
        //target->move(fundamental::calculateDistance(tarNewVel, deltaTime - travelTime));

        std::cout << "      -  After:" << std::endl;
        std::cout << "           -  Object position: " << object->getPosition().x << ", " << object->getPosition().y << ", " << object->getPosition().z << ", target position: " << target->getPosition().x << ", " << target->getPosition().y << ", " << target->getPosition().z << std::endl;
        std::cout << "           -  Object velocity: " << object->getVelocity().x << ", " << object->getVelocity().y << ", " << object->getPosition().z << ", target velocity: " << target->getVelocity().x << ", " << target->getVelocity().y << ", " << target->getVelocity().z << std::endl;

        std::cout << "      -  Other:" << std::endl;
        std::cout << "           -  New object velocity: " << objNewVel.x << ", " << objNewVel.y << ", " << objNewVel.z << std::endl;
        std::cout << "           -  New target velocity: " << tarNewVel.x << ", " << tarNewVel.y << ", " << tarNewVel.z << std::endl;
        std::cout << "           -  Time remaining: " << deltaTime - travelTime << std::endl;

        return deltaTime - travelTime;
    }
    else
    {
        std::cout << "      -  Not collided, Time took: " << std::fixed << std::setprecision(5) << travelTime << ", Delta time: " << deltaTime << std::endl;
        std::cout << "           -  Object position: " << objPos.x << ", " << objPos.y << ", " << objPos.z << ", target position: " << tarPos.x << ", " << tarPos.y << ", " << tarPos.z << std::endl;
    }
    return deltaTime;
}