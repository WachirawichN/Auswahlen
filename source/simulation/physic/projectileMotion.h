#pragma once

#include "../../dependencies/GLM/glm.hpp"

namespace projectileMotion
{
    glm::vec3 calculateVelocity(glm::vec3 acceleration, float deltaTime);
    glm::vec3 calculateDistance(glm::vec3 velocity, float deltaTime);
}