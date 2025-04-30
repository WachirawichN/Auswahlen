#pragma once

#include <vector>

namespace geometry
{
    class shape
    {
        protected:
            std::vector<float> vertices;
            std::vector<unsigned int> indices;
        public:
            __host__ std::vector<float> getVertices()
            {
                return vertices;
            }
            __host__ std::vector<unsigned int> getIndices()
            {
                return indices;
            }
    };
}