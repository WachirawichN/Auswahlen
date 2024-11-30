#pragma once

#include "../object/objectBaseClass.h"
#include "fundamental.h"
#include "momentum.h"

#include <memory>
#include <iomanip>

namespace collision
{
    float dstBaseCD(std::shared_ptr<object::objectBaseClass> currentObject, std::shared_ptr<object::objectBaseClass> target, float deltaTime);
}