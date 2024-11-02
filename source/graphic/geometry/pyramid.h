#pragma once

#include "shape.h"

namespace geometry
{
    class pyramid : public shape
    {
        private:
            float width;
            float height;
            float depth;
        public:
            pyramid(float width, float height, float depth); 
    };
}