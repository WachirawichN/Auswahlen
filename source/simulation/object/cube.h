#pragma once

#include "object.h"
#include "../../graphic/geometry/cube.h"

namespace object
{
    class cube : public geometry::cube, public object::objectBaseClass
    {
        private:
        public:
            cube(float width = 1.0f, float height = 1.0f, float depth = 1.0f, bool collision = true, bool gravity = true, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f))
                : geometry::cube::cube(width, height, depth), object::objectBaseClass(collision, gravity, position, rotation, scale) {}
    };
}
