#pragma once

#include "../../dependencies/GLM/glm.hpp"

#include <cmath>
#include <vector>

namespace mathExt
{
    float pythagoras(glm::vec3 input);
    float roundToDec(float input, unsigned int decimal);
    glm::vec3 roundToDecVec3(glm::vec3 input, unsigned int decimal);
}