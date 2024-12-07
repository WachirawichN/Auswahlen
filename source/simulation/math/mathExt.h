#pragma once

#include "../../dependencies/GLM/glm.hpp"

#include <cmath>

namespace mathExt
{
    float pythagoras(glm::vec3 input);
    float roundToDecimal(float input, unsigned int decimal);
}