#pragma once

#include <vector>

class pyramid
{
    private:
        float width;
        float height;
        float depth;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;
    public:
        pyramid(float width, float height, float depth);

        std::vector<float> getVertices();
        std::vector<unsigned int> getIndices();
};