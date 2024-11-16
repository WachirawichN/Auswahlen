#include "collision.h"

#include <iostream>

float pythagorasTheorem(glm::vec3 dstVector)
{
    return sqrt(pow(dstVector.x, 2) + pow(dstVector.y, 2) + pow(dstVector.z, 2));
}

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

void collision::continuouseCollisionDetection(std::shared_ptr<object::objectBaseClass> currentObject, std::shared_ptr<object::objectBaseClass> target, float deltaTime)
{
    glm::vec3 objectPosition = currentObject->getPosition();
    glm::vec3 objectScale = currentObject->getScale();

    glm::vec3 targetPosition = target->getPosition();
    glm::vec3 targetScale = target->getScale();

    glm::vec3 newObjectPosition = objectPosition;
    glm::vec3 newObjectVelocity = currentObject->getVelocity();

    int axisOfContact = 0;

    for (int axis = 0; axis < 3; axis++)
    {
        float currentObjectSide = objectPosition[axis] - targetPosition[axis];
        float objectAxisBorder = objectScale[axis] / 2;

        // Stepping physic calculation
        float newAxisPosition = objectPosition[axis] + (newObjectVelocity[axis] * deltaTime);

        // Detect collision
        if (currentObjectSide > 0) // Positive border detection (Front, Right, Top)
        {
            float positiveBorder = targetPosition[axis] + (targetScale[axis] / 2);

            // Run when new object position is within positive border
            
            if (newAxisPosition - objectAxisBorder < positiveBorder)
            {
                //std::cout << "Object new border position: " << newAxisPosition - objectAxisBorder << " Positive border: " << positiveBorder << std::endl;
                axisOfContact++;

                // Side from object border to target border
                float oldObjBorderTargetSide = objectPosition[axis] + objectAxisBorder;

                // Execute when object ram into border
                // Check if object border come from the other side of target border
                if (oldObjBorderTargetSide > positiveBorder)
                {
                    //std::cout << "Old object-target side: " << oldObjBorderTargetSide << std::endl;
                    //std::cout << "New object-target side: " << newAxisPosition - objectAxisBorder << std::endl;

                    newObjectPosition[axis] = positiveBorder + (objectScale[axis] / 2);
                    newObjectVelocity[axis] *= -1;

                    //std::cout << "New axis position : " << newObjectPosition[axis] << std::endl;
                }

                // Execute when object is moving in that axis
                /*
                if (newObjectVelocity[axis] != 0.0f)
                {
                    // Side from object border to target border
                    float oldObjBorderTargetSide = (objectPosition[axis] + objectAxisBorder) - targetPosition[axis];
                    float newObjBorderTargetSide = (newAxisPosition + objectAxisBorder) - targetPosition[axis];

                    // Execute when object ram into border
                    if ((oldObjBorderTargetSide < 0) != (newObjBorderTargetSide < 0))
                    {
                        //std::cout << "Ricochet" << std::endl;
                        newObjectPosition[axis] = positiveBorder + (objectScale[axis] / 2);
                        newObjectVelocity[axis] *= -1;
                        std::cout << "New axis position : " << newObjectPosition[axis] << std::endl;
                    }
                }
                */
            }
        }
        else if (currentObjectSide < 0) // Negative border detection (Back, Left, Bottom)
        {
            float negativeBorder = targetPosition[axis] - (targetScale[axis] / 2);

            // Run when new object position is within negative border
            
            if (newAxisPosition + objectAxisBorder > negativeBorder)
            {
                //std::cout << "Object new border position: " << newAxisPosition + objectAxisBorder << " Negative border: " << negativeBorder << std::endl;
                axisOfContact++;

                // Side from object border to target border
                float oldObjBorderTargetSide = objectPosition[axis] + objectAxisBorder;

                // Execute when object ram into border
                // Check if object border come from the other side of target border
                if (oldObjBorderTargetSide < negativeBorder)
                {
                    //std::cout << "Old object-target side: " << oldObjBorderTargetSide << std::endl;
                    //std::cout << "New object-target side: " << newAxisPosition + objectAxisBorder << std::endl;

                    newObjectPosition[axis] = negativeBorder - (objectScale[axis] / 2);
                    newObjectVelocity[axis] *= -1;

                    //std::cout << "New axis position : " << newObjectPosition[axis] << std::endl;
                }

                // Execute when object is moving in that axis
                /*
                if (newObjectVelocity[axis] != 0.0f)
                {
                    // Side from object border to target border
                    float oldObjBorderTargetSide = (objectPosition[axis] + objectAxisBorder) - targetPosition[axis];
                    float newObjBorderTargetSide = (newAxisPosition + objectAxisBorder) - targetPosition[axis];

                    // Execute when object ram into border
                    if ((oldObjBorderTargetSide < 0) != (newObjBorderTargetSide < 0))
                    {
                        //std::cout << "Ricochet" << std::endl;
                        newObjectPosition[axis] = negativeBorder - (objectScale[axis] / 2);
                        newObjectVelocity[axis] *= -1;
                        std::cout << "New axis position : " << newObjectPosition[axis] << std::endl;
                    }
                }
                */
            }
        }
        else
        {
            axisOfContact++;
        }
        
        //std::cout << axis << " : " << axisOfContact << std::endl;
    }

    if (axisOfContact == 3)
    {
        //std::cout << "Original velocity : " << currentObject->getVelocity().x << " " << currentObject->getVelocity().y << " " << currentObject->getVelocity().z << std::endl;
        //currentObject->changeVelocity(-(currentObject->getVelocity()));
        //std::cout << "Neutralize velocity : " << currentObject->getVelocity().x << " " << currentObject->getVelocity().y << " " << currentObject->getVelocity().z << std::endl;
        currentObject->changeVelocity(newObjectVelocity - currentObject->getVelocity());
        //std::cout << "New velocity : " << currentObject->getVelocity().x << " " << currentObject->getVelocity().y << " " << currentObject->getVelocity().z << std::endl;

        glm::vec3 dst(newObjectPosition - objectPosition); // Distance from current object position to the surface of the target
        //std::cout << "Distance : " << dst.x << " " << dst.y << " " << dst.z << std::endl;
        //std::cout << "New position : " << newObjectPosition.x << " " << newObjectPosition.y << " " << newObjectPosition.z << std::endl;
        //std::cout << "Org position : " << objectPosition.x << " " << objectPosition.y << " " << objectPosition.z << std::endl;

        float usageTime = pythagorasTheorem(dst) / pythagorasTheorem(currentObject->getVelocity());
        float leftOverTime = deltaTime - usageTime;
        //std::cout << "Left over time : " << leftOverTime << std::endl;
        glm::vec3 reflectDst = projectileMotion::calculateDistance(newObjectVelocity, leftOverTime);
        //std::cout << "Rerflected distnace : " << reflectDst.x << " " << reflectDst.y << " " << reflectDst.z << std::endl;
        currentObject->move(reflectDst + dst);
        std::cout << "collided" << std::endl;
    }
    else
    {
        // Stepping normal physic calcultion
        glm::vec3 dst = projectileMotion::calculateDistance(currentObject->getVelocity(), deltaTime);
        currentObject->move(dst);
    }

    //std::cout << std::endl;
}

glm::vec3 collision::collisionResolver(glm::vec3 objectVelocity)
{
    return objectVelocity * -1.0f;
}