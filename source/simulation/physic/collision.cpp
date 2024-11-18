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

float collision::continuouseCollisionDetection(std::shared_ptr<object::objectBaseClass> currentObject, std::shared_ptr<object::objectBaseClass> target, float deltaTime)
{
    glm::vec3 objectPosition = currentObject->getPosition();
    glm::vec3 objectScale = currentObject->getScale();

    glm::vec3 targetPosition = target->getPosition();
    glm::vec3 targetScale = target->getScale();

    glm::vec3 newObjectPosition = objectPosition;
    glm::vec3 newObjectVelocity = currentObject->getVelocity();

    int axisOfContact = 0;

    std::cout << "Target X position: " << targetPosition.x << std::endl;

    // Check whether object is clipping with target in each axis
    for (int axis = 0; axis < 3; axis++)
    {
        // Side of object relative to target
        float currentObjectSide = objectPosition[axis] - targetPosition[axis];

        // Distance from object center to axis border
        float objectAxisBorder = objectScale[axis] / 2;

        // Stepping physic calculation
        float newAxisPosition = objectPosition[axis] + (newObjectVelocity[axis] * deltaTime);

        // Detect collision
        if (currentObjectSide > 0) // Positive border detection (Front, Right, Top)
        {
            // Positive border of target
            float positiveBorder = targetPosition[axis] + (targetScale[axis] / 2);

            std::cout << "Axis: " << axis << " Next object axis position: " << newAxisPosition << " Object border position: " << newAxisPosition - objectAxisBorder << " Positive border: " << positiveBorder;
            // Run when new object position is within positive border
            if (newAxisPosition - objectAxisBorder < positiveBorder)
            {
                axisOfContact++;

                // Side from object border to target border
                float oldObjBorderTargetSide = objectPosition[axis] + objectAxisBorder;

                // Execute when object ram into border
                // Check if object border come from the other side of target border
                if (oldObjBorderTargetSide > positiveBorder)
                {
                    newObjectPosition[axis] = positiveBorder + (objectScale[axis] / 2);
                    newObjectVelocity[axis] *= -1;
                }
            }
            std::cout << " Current contact add up: " << axisOfContact << std::endl;
        }
        else if (currentObjectSide < 0) // Negative border detection (Back, Left, Bottom)
        {
            // Negative border of target
            float negativeBorder = targetPosition[axis] - (targetScale[axis] / 2);

            std::cout << "Axis: " << axis << " Next object axis position: " << newAxisPosition << " Object border position: " << newAxisPosition + objectAxisBorder << " Negative border: " << negativeBorder;
            // Run when new object position is within negative border
            if (newAxisPosition + objectAxisBorder > negativeBorder)
            {
                axisOfContact++;

                // Side from object border to target border
                float oldObjBorderTargetSide = objectPosition[axis] + objectAxisBorder;

                // Execute when object ram into border
                // Check if object border come from the other side of target border
                if (oldObjBorderTargetSide < negativeBorder)
                {
                    newObjectPosition[axis] = negativeBorder - (objectScale[axis] / 2);
                    newObjectVelocity[axis] *= -1;
                }
            }
            std::cout << " Current contact add up: " << axisOfContact << std::endl;
        }
        else
        {
            std::cout << "Axis: " << axis << " Side = 0 " << std::endl;
            axisOfContact++;
        }
    }

    std::cout << "Contact: " << axisOfContact << std::endl;
    if (axisOfContact == 3)
    {
        // Multiply velocity with velocity conservation
        newObjectVelocity *= 1.0f;

        // Neutralize old velocity
        currentObject->changeVelocity(-(currentObject->getVelocity()));

        // Move object to the surface of the target
        glm::vec3 toSurfaceDst(newObjectPosition - objectPosition);
        currentObject->move(toSurfaceDst);

        currentObject->changeVelocity(newObjectVelocity);

        // Calculate left over time from traveling to target surface
        float usageTime = pythagorasTheorem(toSurfaceDst) / pythagorasTheorem(newObjectVelocity);
        float leftOverTime = deltaTime - usageTime;

        std::cout << "Collide with object" << std::endl;
        std::cout << "Delta second: " << deltaTime << " second" << std::endl;
        std::cout << "New position: " << newObjectPosition.x << ", " << newObjectPosition.y << ", " << newObjectPosition.z << std::endl;
        std::cout << "New velocity: " << newObjectVelocity.x << ", " << newObjectVelocity.y << ", " << newObjectVelocity.z << std::endl;
        return leftOverTime;
    }
    else return deltaTime;
}

glm::vec3 collision::collisionResolver(glm::vec3 objectVelocity)
{
    return objectVelocity * -1.0f;
}