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

glm::vec3 mathExt::roundToDecVec3(glm::vec3 input, unsigned int decimal)
{
    double multiplier = pow(10.0f, decimal);
    std::vector<float> temp;
    for (int axis = 0; axis < 3; axis++)
    {
        temp.push_back(round(input[axis] * multiplier) / multiplier);
    }
    return glm::vec3(temp[0], temp[1], temp[2]);
}
