#pragma once

#include "../dependencies/GLM/glm.hpp"
#include "../dependencies/GLM/ext.hpp"

#include <vector>
#include <math.h>

class icosphere
{
    private:
        float radius;
        int subdivision;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
    public:
        icosphere(float radius, int subdivision = 1);

        std::vector<float> getVertices();
        std::vector<unsigned int> getIndices();
};