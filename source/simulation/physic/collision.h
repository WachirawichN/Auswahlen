#pragma once

#include "../object/objectBaseClass.h"
#include "fundamental.h"
#include "momentum.h"

#include <vector>
#include <memory>
#include <iomanip>

namespace collision
{
    std::vector<float> collisionPairing();
    float dstBaseCD(std::shared_ptr<object::objectBaseClass> currentObject, std::shared_ptr<object::objectBaseClass> target, float deltaTime);
    void collsionResolver(std::shared_ptr<object::objectBaseClass> currentObject, std::shared_ptr<object::objectBaseClass> target, float travelTime);
}