#pragma once

#include "../../dependencies/GLM/glm.hpp"

#include <cmath>
#include <vector>

namespace mathExt
{
    enum side {
        LEFT = -1,
        MIDDLE = 0,
        RIGHT = 1
    };

    // Basic math
    float pythagoras(glm::vec3 input);
    float roundToDec(float input, unsigned int decimal);

    // Positioning
    side aToBside(float aPos, float bPos);
    side direction(float input);
    
    // Other
    int falseToMinus(bool input);
}