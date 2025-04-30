#pragma once

#include <GLC/utility.cuh>
#include <math.h>
#include "shape.cuh"

namespace geometry
{
    class icosphere : public geometry::shape
    {
        protected:
            float radius;
            int subdivision;
        public:
            __host__ icosphere(float radius, unsigned int subdivision = 1);
            __host__ __device__ float getRadius() const;
    };
}