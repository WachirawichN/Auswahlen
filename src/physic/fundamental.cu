#include "physic/fundamental.cuh"

GLC::vec3 fundamental::calculateVel(GLC::vec3 acceleration, float deltaTime)
{
    return acceleration * deltaTime;
}

GLC::vec3 fundamental::calculateDst(GLC::vec3 velocity, float deltaTime)
{
    return velocity * deltaTime;
}