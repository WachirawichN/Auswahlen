#include "fundamental.cuh"

glm::vec3 fundamental::calculateVel(glm::vec3 acceleration, float deltaTime)
{
    return acceleration * deltaTime;
}

glm::vec3 fundamental::calculateDst(glm::vec3 velocity, float deltaTime)
{
    return velocity * deltaTime;
}