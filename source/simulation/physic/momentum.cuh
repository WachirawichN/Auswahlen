#pragma once

#include "../object/objectBaseClass.cuh"

#include <memory>
#include <vector>

namespace momentum
{
    __device__ float elasticCollision1D(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target, unsigned int axis);
}