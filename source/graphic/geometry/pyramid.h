#pragma once

#include "shape.h"

namespace geometry
{
    class pyramid : public geometry::shape
    {
        protected:
            float width;
            float height;
            float depth;
        public:
            pyramid(float width, float height, float depth); 
    };
}