#pragma once

#include "../object/objectBaseClass.h"
#include <memory>
#include <vector>

namespace momentum
{
    glm::vec3 elasticCollision(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target);
}