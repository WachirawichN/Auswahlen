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

bool collision::continuouseCollisionDetection(std::shared_ptr<object::objectBaseClass> currentObject, std::shared_ptr<object::objectBaseClass> target)
{
    glm::vec3 objectVelocity = currentObject->getVelocity();
    glm::vec3 objectPosition = currentObject->getPosition();
    glm::vec3 targetPosition = target->getPosition();
    for (int axis = 0; axis < 3; axis++)
    {
        float objectAxisPosition = objectPosition[axis];
        float targetAxisPosition = targetPosition[axis];
        float distance = objectAxisPosition - targetAxisPosition;
    }
    return;
}

glm::vec3 collision::collisionResolver(glm::vec3 objectVelocity)
{
    return objectVelocity * -1.0f;
}