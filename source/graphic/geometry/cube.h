#pragma once

#include "shape.h"

namespace geometry
{
    class cube : public geometry::shape
    {
        protected:
            float width;
            float height;
            float depth;
        public:
            cube(float width, float height, float depth);
            float getWidth() const;
            float getDepth() const;
            float getHeight() const;
    };
}

