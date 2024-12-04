#include "collision.h"

#include "../object/sphere.h"
#include <iostream>

float pythagoras(glm::vec3 dstVector)
{
    return sqrt(pow(dstVector.x, 2) + pow(dstVector.y, 2) + pow(dstVector.z, 2));
}
float timeToMove(float aPosition, float bPosition, float aVelocity, float bVelocity)
{
    float totalVelocity = aVelocity - bVelocity;
    float dst = bPosition - aPosition;
    if (totalVelocity == 0.0f) return 0.0f;
    return dst / totalVelocity;
    //return (totalVelocity > 0.0f) ? dst / totalVelocity : ((aPosition - bPosition)) / totalVelocity;
}
glm::vec3 timeToMove3D(glm::vec3 objAPos, glm::vec3 objAVel, glm::vec3 objBPos, glm::vec3 objBVel)
{
    std::vector<float> axisTimes = {};
    unsigned int negativeCount = 0;
    unsigned int maxNegative = 3;
    std::cout << "      -  Distance and velocity:" << std::endl;
    for (int axis = 0; axis < 3; axis++)
    {
        std::cout << "           -  Axis: " << axis << ", Distance: " << objBPos[axis] - objAPos[axis] << ", Total velocity: " << objAVel[axis] - objBVel[axis] << std::endl;

        // If the time is below zero will mean the object and the target will hit each other in the past in that axis
        float time = timeToMove(objAPos[axis], objBPos[axis], objAVel[axis], objBVel[axis]);
        if (time < 0.0f) negativeCount++;
        if (time == 0.0f) maxNegative--;
        
        axisTimes.push_back(time);
    }

    std::cout << "      -  Negative count: " << negativeCount << std::endl;
    std::cout << "      -  Max negative count: " << maxNegative << std::endl;
    std::cout << "      -  axisTime: " << axisTimes[0] << ", " << axisTimes[1] << ", " << axisTimes[2] << std::endl;
    if (negativeCount == 0 || negativeCount == maxNegative)
    {
        // Object and target will hit each other in the future
        return glm::vec3(axisTimes[0], axisTimes[1], axisTimes[2]);
    }
    // Object and the target will never hit each other
    return glm::vec3(NULL, NULL, NULL);
}

float collision::dstBaseCD(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target, float deltaTime)
{
    glm::vec3 objPos = object->getPosition();
    glm::vec3 objScale = object->getScale();
    glm::vec3 objVel = object->getVelocity();

    glm::vec3 tarPos = target->getPosition();
    glm::vec3 tarScale = target->getScale();
    glm::vec3 tarVel = target->getVelocity();

    std::cout << "      -  Scaling: " << std::endl;
    std::cout << "           -  Object: " << objScale[0] << ", " << objScale[1] << ", " << objScale[2] << std::endl;
    std::cout << "           -  Target: " << tarScale[0] << ", " << tarScale[1] << ", " << tarScale[2] << std::endl;
    std::cout << "      -  Origin position: " << std::endl;
    std::cout << "           -  Object: " << objPos[0] << ", " << objPos[1] << ", " << objPos[2] << std::endl;
    std::cout << "           -  Target: " << tarPos[0] << ", " << tarPos[1] << ", " << tarPos[2] << std::endl;

    // Stretch out border of object and target according to what side of object is correlate to target on all axis
    for (int axis = 0; axis < 3; axis++)
    {
        float side = objPos[axis] - tarPos[axis];
        if (side > 0)
        {
            // Object is on positive side compare to target position
            tarPos[axis] += tarScale[axis] / 2;

            // Calculate object border according to if the object is within target border or not
            side = objPos[axis] - tarPos[axis];
            if (side > 0)
            {
                objPos[axis] -= objScale[axis] / 2;
            }
            else if (side < 0)
            {
                objPos[axis] += objScale[axis] / 2;
            }
        }
        else if (side < 0)
        {
            // Object is on negative side compare to target position
            tarPos[axis] -= tarScale[axis] / 2;

            // Calculate object border according to if the object is within target border or not
            side = objPos[axis] - tarPos[axis];
            if (side > 0)
            {
                objPos[axis] -= objScale[axis] / 2;
            }
            else if (side < 0)
            {
                objPos[axis] += objScale[axis] / 2;
            }
        }
        
    }

    std::cout << "      -  Border position: " << std::endl;
    std::cout << "           -  Object: " << objPos[0] << ", " << objPos[1] << ", " << objPos[2] << std::endl;
    std::cout << "           -  Target: " << tarPos[0] << ", " << tarPos[1] << ", " << tarPos[2] << std::endl;
    
    glm::vec3 travelTimes = timeToMove3D(objPos, objVel, tarPos, tarVel);
    std::cout << "      -  Time comparison: " << std::endl;
    unsigned int collideAxis = 0;
    for (int axis = 0; axis < 3; axis++)
    {
        std::cout << "           -  Axis: " << axis << std::endl;
        std::cout << "                -  deltaTime > 0.0f: " << (deltaTime > 0.0f) << ", travelTimes > 0.0f: " << (travelTimes[axis] > 0.0f) << ", travelTimes < deltaTime: " << (travelTimes[axis] < deltaTime) << ", travelTimes == 0.0f: " << (travelTimes[axis] == 0.0f) << std::endl;
        std::cout << "                -  deltaTime < 0.0f: " << (deltaTime < 0.0f) << ", travelTimes < 0.0f: " << (travelTimes[axis] < 0.0f) << ", travelTimes > deltaTime: " << (travelTimes[axis] > deltaTime) << ", travelTimes == 0.0f: " << (travelTimes[axis] == 0.0f) << std::endl;
        if (travelTimes[axis] == 0.0f)
        {
            collideAxis++;
            std::cout << "                -  contact: " << collideAxis << std::endl;
            std::cout << "                -  travelTimes: " << travelTimes[axis] << std::endl;
            continue;
        }
        if ((deltaTime > 0.0f && travelTimes[axis] < deltaTime && travelTimes[axis] > 0.0f) || (deltaTime < 0.0f && travelTimes[axis] > deltaTime && travelTimes[axis] < 0.0f))
        {
            collideAxis++;
        }
        std::cout << "                -  contact: " << collideAxis << std::endl;
    }

    //float travelTime = pythagoras(travelTimes);

    //std::cout << "      -  Before:" << std::endl;
    //std::cout << "           -  Object border position: " << objPos.x << ", " << objPos.y << ", " << objPos.z << ", target border position: " << tarPos.x << ", " << tarPos.y << ", " << tarPos.z << std::endl;
    //std::cout << "           -  Object velocity: " << objVel.x << ", " << objVel.y << ", " << objVel.z << ", target velocity: " << tarVel.x << ", " << tarVel.y << ", " << tarVel.z << std::endl;

    if (collideAxis == 3)
    {
        std::cout << "      -  Very collided" << std::endl;
    }
    else
    {
        std::cout << "      -  Not so collided" << std::endl;
    }
    

    // Object did collide with target
    /*
    if ((deltaTime > 0.0f && travelTime < deltaTime && travelTime > 0.0f) || (deltaTime < 0.0f && travelTime > deltaTime && travelTime < 0.0f))
    {
        std::cout << "      -  Got collided, Time took: " << std::fixed << std::setprecision(5) << travelTime << ", Delta time: " << deltaTime << std::endl;

        // Move both object to the position that it only contact each other
        object->move(fundamental::calculateDstVecT(objVel, travelTimes));
        target->move(fundamental::calculateDstVecT(tarVel, travelTimes));

        glm::vec3 objNewVel = momentum::elasticCollision(object, target);
        glm::vec3 tarNewVel = momentum::elasticCollision(target, object);
        object->changeVelocity(objNewVel - objVel);
        target->changeVelocity(tarNewVel - tarVel);

        //object->move(fundamental::calculateDst(objNewVel, deltaTime - travelTime));
        //target->move(fundamental::calculateDst(tarNewVel, deltaTime - travelTime));

        std::cout << "      -  After:" << std::endl;
        std::cout << "           -  Object position: " << object->getPosition().x << ", " << object->getPosition().y << ", " << object->getPosition().z << ", target position: " << target->getPosition().x << ", " << target->getPosition().y << ", " << target->getPosition().z << std::endl;
        std::cout << "           -  Object velocity: " << object->getVelocity().x << ", " << object->getVelocity().y << ", " << object->getPosition().z << ", target velocity: " << target->getVelocity().x << ", " << target->getVelocity().y << ", " << target->getVelocity().z << std::endl;

        std::cout << "      -  Other:" << std::endl;
        std::cout << "           -  New object velocity: " << objNewVel.x << ", " << objNewVel.y << ", " << objNewVel.z << std::endl;
        std::cout << "           -  New target velocity: " << tarNewVel.x << ", " << tarNewVel.y << ", " << tarNewVel.z << std::endl;
        std::cout << "           -  Time remaining: " << deltaTime - travelTime << std::endl;

        //return 0.0f;
        return deltaTime - travelTime;
    }
    else
    {
        std::cout << "      -  Not collided, Time took: " << std::fixed << std::setprecision(5) << travelTime << ", Delta time: " << deltaTime << std::endl;
        std::cout << "           -  Object position: " << objPos.x << ", " << objPos.y << ", " << objPos.z << ", target position: " << tarPos.x << ", " << tarPos.y << ", " << tarPos.z << std::endl;
    }
    */
    return deltaTime;
}

void collision::collsionResolver(std::shared_ptr<object::objectBaseClass> currentObject, std::shared_ptr<object::objectBaseClass> target, float travelTime)
{

}
