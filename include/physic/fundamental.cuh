#pragma once

#include "GLC/matrix.cuh"
#include "GLC/vector.cuh"
#include "GLC/utility.cuh"

#include <vector>

namespace fundamental
{
    GLC::vec3 calculateVel(GLC::vec3 acceleration, float deltaTime);
    GLC::vec3 calculateDst(GLC::vec3 velocity, float deltaTime);
}