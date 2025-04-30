#pragma once

#include "shape.cuh"

namespace geometry
{
    class cube : public geometry::shape
    {
        protected:
            float width;
            float height;
            float depth;
        public:
            __host__ cube(float width, float height, float depth);
            __host__ __device__ float getWidth() const;
            __host__ __device__ float getDepth() const;
            __host__ __device__ float getHeight() const;
    };
}

