#include "../../dependencies/GLM/glm.hpp"

glm::vec3 calculateVelocity(glm::vec3 acceleration, float deltaTime)
{
    glm::vec3 deltaVelocity = acceleration * deltaTime;
    return deltaVelocity;
}

glm::vec3 calculateDistance(glm::vec3 velocity, float deltaTime)
{
    glm::vec3 distance = velocity * deltaTime;
    return distance;
}