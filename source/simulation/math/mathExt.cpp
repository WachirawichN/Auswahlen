#include "mathExt.h"

float mathExt::pythagoras(glm::vec3 input)
{
    return sqrt(pow(input.x, 2) + pow(input.y, 2) + pow(input.z, 2));
}

float mathExt::roundToDecimal(float input, unsigned int decimal)
{
    double multiplier = pow(10.0f, decimal);
    return round(input * multiplier) / multiplier;
}