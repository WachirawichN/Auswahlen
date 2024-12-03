#pragma once

#include "../../dependencies/GLM/glm.hpp"

#include <vector>

namespace fundamental
{
    glm::vec3 calculateVel(glm::vec3 acceleration, float deltaTime);
    glm::vec3 calculateDst(glm::vec3 velocity, float deltaTime);
    glm::vec3 calculateDstVecT(glm::vec3 velocity, glm::vec3 deltaTime);
}