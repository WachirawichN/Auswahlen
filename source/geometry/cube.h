#pragma once

#include <vector>

class cube
{
    private:
        float width;
        float height;
        float depth;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;
    public:
        cube(float width, float height, float depth);

        std::vector<float> getVertices();
        std::vector<unsigned int> getIndices();
};

