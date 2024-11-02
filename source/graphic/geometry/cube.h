#pragma once

#include "shape.h"

namespace geometry
{
    class cube : public shape
    {
        private:
            float width;
            float height;
            float depth;
        public:
            cube(float width, float height, float depth);
    };
}

