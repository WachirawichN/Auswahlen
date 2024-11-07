#pragma once

#include "../object/objectBaseClass.h"
#include <memory>

namespace collision
{
    glm::vec3 sphereBoxCollision(std::shared_ptr<object::objectBaseClass> sphere, std::shared_ptr<object::objectBaseClass> box);
    glm::vec3 sphereSphereCollision(std::shared_ptr<object::objectBaseClass> sphere1, std::shared_ptr<object::objectBaseClass> sphere2);
}