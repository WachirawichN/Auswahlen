#include "mathExt.h"

float mathExt::pythagoras(glm::vec3 input)
{
    return sqrt(pow(input.x, 2) + pow(input.y, 2) + pow(input.z, 2));
}
float mathExt::roundToDec(float input, unsigned int decimal)
{
    double multiplier = pow(10.0f, decimal);
    return round(input * multiplier) / multiplier;
}

mathExt::side mathExt::aToBside(float aPos, float bPos)
{
    float delta = aPos - bPos;
    return (delta == 0.0f) ? MIDDLE : (delta > 0.0f) ? LEFT : RIGHT;
}
mathExt::side mathExt::direction(float input)
{
    return (input == 0.0f) ? MIDDLE : (input > 0.0f) ? RIGHT : LEFT;
}

int mathExt::falseToMinus(bool input)
{
    return (input == 0) ? -1 : 1;
}