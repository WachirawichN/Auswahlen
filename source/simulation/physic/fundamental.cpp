#include "fundamental.h"

glm::vec3 fundamental::calculateVel(glm::vec3 acceleration, float deltaTime)
{
    return acceleration * deltaTime;
}

glm::vec3 fundamental::calculateDst(glm::vec3 velocity, float deltaTime)
{
    return velocity * deltaTime;
}

glm::vec3 fundamental::calculateDstVecT(glm::vec3 velocity, glm::vec3 deltaTime)
{
    std::vector<float> dst;
    for (int axis = 0; axis < 3; axis++)
    {
        dst.push_back(velocity[axis] * deltaTime[axis]);
    }
    return glm::vec3(dst[0], dst[1], dst[2]);
}