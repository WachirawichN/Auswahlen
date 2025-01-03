#pragma once

#include "../object/objectBaseClass.h"
#include "../object/sphere.h"
#include "fundamental.h"
#include "momentum.h"
#include "hitbox.h"
#include "../math/mathExt.h"

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <iomanip>

namespace collision
{
    enum collisionType {
        NO,
        NEWLY,
        CROSS,
        INSIDE
    };

    std::vector<std::vector<unsigned int>> collisionPairing(std::vector<std::shared_ptr<object::objectBaseClass>> objects);
    std::vector<collisionType> CCD(std::shared_ptr<object::objectBaseClass> obj, std::shared_ptr<object::objectBaseClass> tar, float deltaTime);
    float collisionResolver(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target, float deltaTime, std::vector<unsigned int> newlyAxis);
}