#include "fundamental.h"

glm::vec3 fundamental::calculateVelocity(glm::vec3 acceleration, float deltaTime)
{
    return acceleration * deltaTime;
}

glm::vec3 fundamental::calculateDistance(glm::vec3 velocity, float deltaTime)
{
    return velocity * deltaTime;
}