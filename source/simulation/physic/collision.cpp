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
    return (totalVelocity == 0.0f) ? 0.0f : (dst / (aVelocity - bVelocity));
}

float timeToMove3D(glm::vec3 objAPos, glm::vec3 objAVel, glm::vec3 objBPos, glm::vec3 objBVel)
{
    std::vector<float> axisTimes = {};
    for (int axis = 0; axis < 3; axis++)
    {
        float dst = abs(objAPos[axis] - objBPos[axis]);
        float time = timeToMove(dst, objAVel[axis], objBVel[axis]);
        if (time < 0.0f) return -1.0f;
        
        axisTimes.push_back(time);
    }
    return pythagoras(glm::vec3(axisTimes[0], axisTimes[1], axisTimes[2]));
}

bool checkAxisCollision(float objPos, float objDim, float objDst, float tarPos, float tarDim, float tarDst)
{
    // How it work
    // Step 1: Check which side of the object is on according to target position in specific axis
    // Step 2: Calculate the border of target according to which side the object is on (Front, Right, Top = Positive side and Back, Left, Bottom = Negative side)
    // Step 3: Calculate the opposite border of the object (if the target side is Positive then the object will calculate the Negative and vice versa)
    // Step 4: If the object border is elapse with the target border after calculate both new position then this axis is collided

    // Calculate which side of object is on
    float side = objPos - tarPos;
    if (side > 0)
    {
        // Object is on positive side compare to target position
        float newTarNegativeBorder = tarPos - (tarDim / 2) + tarDst;
        float newObjPositiveBorder = objPos + (objDim / 2) + objDst;

        if (newTarNegativeBorder < newObjPositiveBorder) 
        {
            return true;
        }
    }
    else if (side < 0)
    {
        // Object is on negative side compare to target position
        float newTarPositiveBorder = tarPos + (tarDim / 2) + tarDst;
        float newObjNegativeBorder = objPos - (objDim / 2) + objDst;

        if (newTarPositiveBorder > newObjNegativeBorder)
        {
            return true;
        }
    }
    else
    {
        return true;
    }
    return false;
    
}

/*
bool collision::sphereBoxCollision(std::shared_ptr<object::objectBaseClass> sphere, std::shared_ptr<object::objectBaseClass> box)
{
    int axisOfContact = 0;

    glm::vec3 spherePosition = sphere->getPosition();
    glm::vec3 sphereScale = sphere->getScale();

    glm::vec3 boxPosition = box->getPosition();
    glm::vec3 boxScale = box->getScale();
    for (int i = 0; i < 3; i++) // Loop through axis
    {
        float sphereStart = spherePosition[i] - sphereScale[i];
        float sphereEnd = spherePosition[i] + sphereScale[i];

        // If the box is rotate then it will definietly broke
        float boxStart = boxPosition[i] - (boxScale[i] / 2);
        float boxEnd = boxPosition[i] + (boxScale[i] / 2);
        
        if ((sphereStart > boxStart && sphereStart < boxEnd) || (sphereEnd < boxStart && sphereEnd > boxEnd))
        {
            axisOfContact++;
        }
    }
    return ((axisOfContact == 3) ? true : false);
}
bool collision::sphereSphereCollision(std::shared_ptr<object::objectBaseClass> sphere1, std::shared_ptr<object::objectBaseClass> sphere2)
{
    glm::vec3 dst = sphere1->getPosition() - sphere2->getPosition();
    float dstLength = pythagorasTheorem(dst);

    
    geometry::icosphere* shapePtr1 = dynamic_cast<geometry::icosphere*>(sphere1.get());
    float sphere1Radius = shapePtr1->getRadius();
    glm::vec3 sphereScale1 = sphere1->getScale();

    geometry::icosphere* shapePtr2 = dynamic_cast<geometry::icosphere*>(sphere2.get());
    float sphere2Radius = shapePtr2->getRadius();
    glm::vec3 sphereScale2 = sphere2->getScale();

    return (dstLength < (sphere1Radius * sphereScale1.x + sphere2Radius * sphereScale2.x)) ? true : false;
}
*/

float collision::continuouseCollisionDetection(std::shared_ptr<object::objectBaseClass> currentObject, std::shared_ptr<object::objectBaseClass> target, float deltaTime, float energyConversion)
{
    glm::vec3 objectPosition = currentObject->getPosition();
    glm::vec3 objScale = currentObject->getScale();

    glm::vec3 targetPosition = target->getPosition();
    glm::vec3 tarScale = target->getScale();

    glm::vec3 newObjPos = objectPosition;
    glm::vec3 newObjectVelocity = currentObject->getVelocity();

    int axisOfContact = 0;

    // Check whether object is clipping with target in each axis
    for (int axis = 0; axis < 3; axis++)
    {
        // Side of object relative to target
        float currentObjectSide = objectPosition[axis] - targetPosition[axis];

        // Distance from object center to axis border
        float objectAxisBorder = objScale[axis] / 2;

        // Stepping physic calculation
        float newAxisPosition = objectPosition[axis] + (newObjectVelocity[axis] * deltaTime);

        // Detect collision
        if (currentObjectSide > 0) // Positive border detection (Front, Right, Top)
        {
            // Positive border of target
            float targetPositiveBorder = targetPosition[axis] + (tarScale[axis] / 2);

            // Run when new object position is within positive border
            if (newAxisPosition - objectAxisBorder < targetPositiveBorder)
            {
                axisOfContact++;

                // Side from object border to target border
                float objectBorder = objectPosition[axis] + objectAxisBorder;

                // Execute when object ram into border
                // Check if object border come from the other side of target border
                if (objectBorder > targetPositiveBorder)
                {
                    newObjPos[axis] = targetPositiveBorder + (objScale[axis] / 2);
                    newObjectVelocity[axis] *= -1;
                }
            }
        }
        else if (currentObjectSide < 0) // Negative border detection (Back, Left, Bottom)
        {
            // Negative border of target
            float targetNegativeBorder = targetPosition[axis] - (tarScale[axis] / 2);

            // Run when new object position is within negative border
            if (newAxisPosition + objectAxisBorder > targetNegativeBorder)
            {
                axisOfContact++;

                // Side from object border to target border
                float objectBorder = objectPosition[axis] + objectAxisBorder;

                // Execute when object ram into border
                // Check if object border come from the other side of target border
                if (objectBorder < targetNegativeBorder)
                {
                    newObjPos[axis] = targetNegativeBorder - (objScale[axis] / 2);
                    newObjectVelocity[axis] *= -1;
                }
            }
        }
        else
        {
            axisOfContact++;
        }
    }

    if (axisOfContact == 3)
    {
        // Multiply velocity with energy conservation
        newObjectVelocity *= energyConversion;

        // Neutralize old velocity
        currentObject->changeVelocity(-(currentObject->getVelocity()));
        currentObject->changeVelocity(newObjectVelocity);

        // Move object to the surface of the target
        glm::vec3 toSurfaceDst(newObjPos - objectPosition);
        currentObject->move(toSurfaceDst);


        // Calculate left over time from traveling to target surface
        float usageTime = pythagoras(toSurfaceDst) / pythagoras(newObjectVelocity);
        // Sometimes usageTime might be more than deltaTime, might cause some problem later
        float leftOverTime = deltaTime - usageTime;

        if (leftOverTime < 0)
        {
            std::cout << "Insufficient time: " << leftOverTime << " New velocity: " << newObjectVelocity.x << " " << newObjectVelocity.y << " " << newObjectVelocity.z << std::endl;
            return 0.0f;
            /*
            std::cout << "Distance: " << pythagorasTheorem(toSurfaceDst) << " Velocity: " << pythagorasTheorem(newObjectVelocity) << std::endl;
            std::cout << "Delta time: " << deltaTime << " Usage time: " << usageTime << std::endl;
            std::cout << "Usage time: " << toSurfaceDst.x / newObjectVelocity.x << " " << toSurfaceDst.y / newObjectVelocity.y << " " << toSurfaceDst.z / newObjectVelocity.z << std::endl;
            std::cout << "Position: " << currentObject->getPosition().x << " " << currentObject->getPosition().y << " " << currentObject->getPosition().z << std::endl;
            std::cout << "Velocity: " << newObjectVelocity.x << " " << newObjectVelocity.y << " " << newObjectVelocity.z << std::endl;
            std::cout << "Distance: " << toSurfaceDst.x << " " << toSurfaceDst.y << " " << toSurfaceDst.z << std::endl;
            currentObject->setAnchored(true);
            currentObject->setCollision(false);
            */
        }

        //std::cout << "Collide, Left over time: " << leftOverTime << " New velocity: " << newObjectVelocity.x << " " << newObjectVelocity.y << " " << newObjectVelocity.z << " New position: " << newObjPos.x << " " << newObjPos.y << " " << newObjPos.z << std::endl;

        return leftOverTime;
    }
    else
    {
        //std::cout << "No collide, Time:" << deltaTime << ", Current position: " << newObjPos.x << ", " << newObjPos.y << ", " << newObjPos.z << ", Contact: " << axisOfContact << std::endl;
        return deltaTime;
    }
}

float collision::testCollisionDetection(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target, float deltaTime)
{
    glm::vec3 objPos = object->getPosition();
    glm::vec3 objScale = object->getScale();
    glm::vec3 objVel = object->getVelocity();

    glm::vec3 tarPos = target->getPosition();
    glm::vec3 tarScale = target->getScale();
    glm::vec3 tarVel = target->getVelocity();

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

    //float distance = pythagoras(objPos - tarPos);
    //float travelTime = distance / pythagoras(objVel - tarVel);
    float travelTime = timeToMove3D(objPos, objVel, tarPos, tarVel);

    if (!object->isAnchored())
    {
        if (travelTime > deltaTime && travelTime > 0.0f)
        {
            // Did not collide
            object->move(fundamental::calculateDistance(objVel, deltaTime));
            target->move(fundamental::calculateDistance(tarVel, deltaTime));
        }
        else if (travelTime > 0.0f)
        {
            // Did collide
            std::cout << "Time took: " << travelTime << std::endl;
            //std::cout << "Collide" << std::endl;
            object->move(fundamental::calculateDistance(objVel, travelTime));
            target->move(fundamental::calculateDistance(tarVel, travelTime));


            // Debugging
            /*
            object->changeVelocity(-objVel);
            target->changeVelocity(-tarVel);

            object->setAnchored(true);
            target->setAnchored(true);
            */


            glm::vec3 objNewVel = momentum::elasticCollision(object, target);
            glm::vec3 tarNewVel = momentum::elasticCollision(target, object);

            object->changeVelocity(objNewVel - objVel);
            target->changeVelocity(tarNewVel - tarVel);
            
            //std::cout << "Distance: " << distance << std::endl;
            std::cout << "Obj new vel: " << object->getVelocity().x << " " << object->getVelocity().y << " " << object->getVelocity().z << std::endl;
            std::cout << "Tar new vel: " << target->getVelocity().x << " " << target->getVelocity().y << " " << target->getVelocity().z << std::endl;

            std::cout << "Obj new pos: " << object->getPosition().x << " " << object->getPosition().y << " " << object->getPosition().z << std::endl;
            std::cout << "Tar new pos: " << target->getPosition().x << " " << target->getPosition().y << " " << target->getPosition().z << std::endl;

            object->move(fundamental::calculateDistance(objNewVel, deltaTime - travelTime));
            target->move(fundamental::calculateDistance(tarNewVel, deltaTime - travelTime));


            // Debugging

            //object->changeVelocity(-objNewVel);
            //target->changeVelocity(-tarNewVel);

            //object->setAnchored(true);
            //target->setAnchored(true);
            
        }
        return 0.0f;
    }
    return deltaTime;
}