#pragma once

#include <vector>
#include "../math/mathExt.cuh"

namespace hitbox
{
    std::vector<float> sphereHitbox(float position, float radius);
    std::vector<float> cubeHitbox(float position, float scale);
    std::vector<int> hitboxSide(float objPos, float objVel, float tarPos, float tarVel, bool tarInObj, bool objInTar, float timeMultiplier);
}