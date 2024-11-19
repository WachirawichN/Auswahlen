#pragma once

#include "../object/objectBaseClass.h"
#include "projectileMotion.h"
#include <memory>

namespace collision
{
    // Return true if object is colliding
    bool sphereBoxCollision(std::shared_ptr<object::objectBaseClass> sphere, std::shared_ptr<object::objectBaseClass> box);
    bool sphereSphereCollision(std::shared_ptr<object::objectBaseClass> sphere1, std::shared_ptr<object::objectBaseClass> sphere2);
    float continuouseCollisionDetection(std::shared_ptr<object::objectBaseClass> currentObject, std::shared_ptr<object::objectBaseClass> target, float deltaTime, float energyConversion);

    glm::vec3 collisionResolver(glm::vec3 objectVelocity);
}