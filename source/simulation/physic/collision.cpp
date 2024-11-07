#include "collision.h"

#include <iostream>

float pythagorasTheorem(glm::vec3 dstVector)
{
    float sum = 0;
    for (int i = 0; i > 3; i++)
    {
        sum += pow(dstVector[i], 2);
    }
    return sqrt(sum);
}

bool collision::sphereBoxCollision(std::shared_ptr<object::objectBaseClass> sphere, std::shared_ptr<object::objectBaseClass> box)
{
    int axisOfContact = 0;

    glm::vec3 spherePosition = sphere->getPosition();
    glm::vec3 sphereScale = sphere->getScale();
    //geometry::icosphere* shapePtr = dynamic_cast<geometry::icosphere*>(sphere.get());
    //float sphereRadius = shapePtr->getRadius();

    glm::vec3 boxPosition = box->getPosition();
    glm::vec3 boxScale = box->getScale();
    //geometry::cube* boxPtr = dynamic_cast<geometry::cube*>(box.get());
    //float boxDimensions[] = {boxPtr->getWidth(), boxPtr->getDepth(), boxPtr->getHeight()};
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
    glm::vec3 dst = sphere1->getPosition() + sphere2->getPosition();
    float dstLength = pythagorasTheorem(dst);
    
    geometry::icosphere* shapePtr1 = dynamic_cast<geometry::icosphere*>(sphere1.get());
    float sphere1Radius = shapePtr1->getRadius();
    geometry::icosphere* shapePtr2 = dynamic_cast<geometry::icosphere*>(sphere2.get());
    float sphere2Radius = shapePtr2->getRadius();

    return (dstLength < sphere1Radius + sphere2Radius) ? true : false;
}

glm::vec3 collision::collisionResolver(glm::vec3 objectVelocity)
{
    return objectVelocity * -1.0f;
}