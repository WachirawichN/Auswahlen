#pragma once

#include "../object/objectBaseClass.h"

#include <memory>
#include <vector>

namespace momentum
{
    float elasticCollision1D(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target, unsigned int axis);
}