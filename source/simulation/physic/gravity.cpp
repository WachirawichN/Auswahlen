#include "../../dependencies/GLM/glm.hpp"

float gravity(glm::vec3 objectPosition, float acceleration, float deltaTime)
{
    float newY = objectPosition.y - ((acceleration * deltaTime * deltaTime) / 2);
    return newY;
}