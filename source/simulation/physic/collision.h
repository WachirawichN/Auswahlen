#pragma once

#include "../object/objectBaseClass.h"
#include "../object/sphere.h"
#include "fundamental.h"
#include "momentum.h"
#include "../math/mathExt.h"

#include <iostream>
#include <vector>
#include <memory>

namespace collision
{
    std::vector<std::vector<unsigned int>> collisionPairing(std::vector<std::shared_ptr<object::objectBaseClass>> objects);
    glm::vec3 dstBaseCD(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target, glm::vec3 deltaTime);
    void collsionResolver(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target, glm::vec3 travelTime);
}