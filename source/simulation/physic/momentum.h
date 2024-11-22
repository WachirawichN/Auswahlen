#pragma once

#include "../object/objectBaseClass.h"
#include <memory>
#include <vector>

namespace momentum
{
    void elasticCollision(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target);
}