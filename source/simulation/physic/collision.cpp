#include "collision.h"

glm::vec3 collision::sphereBoxCollision(std::shared_ptr<object::objectBaseClass> sphere, std::shared_ptr<object::objectBaseClass> box)
{
    glm::vec3 spherePosition = sphere->getPosition();
    geometry::icosphere* shapePtr = dynamic_cast<geometry::icosphere*>(sphere.get());
    float sphereRadius = shapePtr->getRadius();

    glm::vec3 boxPosition = box->getPosition();
    geometry::cube* boxPtr = dynamic_cast<geometry::cube*>(box.get());
    float boxDimensions[] = {boxPtr->getWidth(), boxPtr->getDepth(), boxPtr->getHeight()};
    for (int i = 0; i < 3; i++) // Loop through axis
    {
        float sphereStart = spherePosition[i] - sphereRadius;
        float sphereEnd = spherePosition[i] + sphereRadius;

        
    }
}

glm::vec3 collision::sphereSphereCollision(std::shared_ptr<object::objectBaseClass> sphere1, std::shared_ptr<object::objectBaseClass> sphere2)
{

}