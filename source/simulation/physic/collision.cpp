#include "collision.h"

#include "../object/sphere.h"
#include <iostream>

float pythagoras(glm::vec3 dstVector)
{
    return sqrt(pow(dstVector.x, 2) + pow(dstVector.y, 2) + pow(dstVector.z, 2));
}

float timeToMove(float dst, float aVelocity, float bVelocity)
{
    float totalVelocity = aVelocity - bVelocity;
    return (totalVelocity == 0.0f) ? 0.0f : (dst / (bVelocity - aVelocity));
}

float timeToMove3D(glm::vec3 objAPos, glm::vec3 objAVel, glm::vec3 objBPos, glm::vec3 objBVel)
{
    std::vector<float> axisTimes = {};
    for (int axis = 0; axis < 3; axis++)
    {
        float dst = objAPos[axis] - objBPos[axis];

        // If the time is zero or below will mean two object will never hit each other in the future but will hit in the past
        float time = timeToMove(dst, objAVel[axis], objBVel[axis]);
        if (time < 0.0f)
        {
            std::cout << "    A velocity: " << objAVel[axis] << ", B velocity" << objBVel[axis] << std::endl;
            std::cout << "    Distance: " << dst << ", Sum velocity: " << objBVel[axis] - objAVel[axis] << std::endl;
            return time;
        }
        
        axisTimes.push_back(time);
    }
    return pythagoras(glm::vec3(axisTimes[0], axisTimes[1], axisTimes[2]));
}

float collision::dstBaseCD(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target, float deltaTime)
{
    glm::vec3 objPos = object->getPosition();
    glm::vec3 objScale = object->getScale();
    glm::vec3 objVel = object->getVelocity();

    glm::vec3 tarPos = target->getPosition();
    glm::vec3 tarScale = target->getScale();
    glm::vec3 tarVel = target->getVelocity();

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
    if (travelTime < deltaTime && travelTime > 0.0f)
    {
        std::cout << "    Object position: " << objPos.x << ", " << objPos.y << ", " << objPos.z << ", target position: " << tarPos.x << ", " << tarPos.y << ", " << tarPos.z << std::endl;
        std::cout << "    Got collided, Time took: " << std::fixed << std::setprecision(5) << travelTime << ", Delta time: " << deltaTime << std::endl;
        object->move(fundamental::calculateDistance(objVel, travelTime));
        target->move(fundamental::calculateDistance(tarVel, travelTime));

        glm::vec3 objNewVel = momentum::elasticCollision(object, target);
        glm::vec3 tarNewVel = momentum::elasticCollision(target, object);

        object->changeVelocity(objNewVel - objVel);
        target->changeVelocity(tarNewVel - tarVel);

        object->move(fundamental::calculateDistance(objNewVel, deltaTime - travelTime));
        target->move(fundamental::calculateDistance(tarNewVel, deltaTime - travelTime));

        return 0.0f;
    }
    else
    {
        std::cout << "    Object position: " << objPos.x << ", " << objPos.y << ", " << objPos.z << ", target position: " << tarPos.x << ", " << tarPos.y << ", " << tarPos.z << std::endl;
        std::cout << "    Not collided, Time took: " << std::fixed << std::setprecision(5) << travelTime << ", Delta time: " << deltaTime << std::endl;
    }
    return deltaTime;
}