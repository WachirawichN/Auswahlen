#include "projectileMotion.h"

glm::vec3 projectileMotion::calculateVelocity(glm::vec3 acceleration, float deltaTime)
{
    return acceleration * deltaTime;
}

glm::vec3 projectileMotion::calculateDistance(glm::vec3 velocity, float deltaTime)
{
    return velocity * deltaTime;
}