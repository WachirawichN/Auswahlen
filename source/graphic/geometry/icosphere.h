#pragma once

#include "../../dependencies/GLM/glm.hpp"
#include "../../dependencies/GLM/gtc/type_ptr.hpp"

#include "shape.h"
#include <math.h>

namespace geometry
{
    class icosphere : public geometry::shape
    {
        protected:
            float radius;
            int subdivision;
        public:
            icosphere(float radius, int subdivision = 1);
    };
}