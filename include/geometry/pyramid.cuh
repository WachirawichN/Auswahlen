#pragma once

#include "shape.cuh"

namespace geometry
{
    class pyramid : public geometry::shape
    {
        protected:
            float width;
            float height;
            float depth;
        public:
            __host__ pyramid(float width, float height, float depth); 
    };
}