#pragma once

#include "../object/objectBaseClass.h"
#include "../object/sphere.h"
#include "fundamental.h"
#include "momentum.h"
#include "../math/mathExt.h"

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

namespace collision
{
    enum collisionType {
        NO,
        NEWLY,
        ALREADY,
        INSIDE
    };

    std::vector<std::vector<unsigned int>> collisionPairing(std::vector<std::shared_ptr<object::objectBaseClass>> objects);
    std::vector<collisionType> dstBaseCD(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target, float deltaTime);
    float collsionResolver(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target, float deltaTime, std::vector<unsigned int> axis);
}