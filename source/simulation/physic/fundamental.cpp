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
    glm::vec3 dstVec;
    for (int axis = 0; axis < 3; axis++)
    {
        dstVec[axis] = velocity[axis] * deltaTime[axis];
    }
    return dstVec;
}