#pragma once

#include <vector>

class shape
{
    protected:
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
    public:
        std::vector<float> getVertices()
        {
            return vertices;
        }
        std::vector<unsigned int> getIndices()
        {
            return indices;
        }
};