#pragma once

#include "objectBaseClass.h"
#include "../../graphic/geometry/cube.h"

namespace object
{
    class cube : public geometry::cube, public object::objectBaseClass
    {
        private:
        public:
            cube(bool collision = true,
                 bool anchored = false,
                 float mass = 1,
                 glm::vec3 velocity = glm::vec3(0.0f),
                 glm::vec3 position = glm::vec3(0.0f),
                 glm::vec3 rotation = glm::vec3(0.0f),
                 glm::vec3 scale = glm::vec3(1.0f),
                 glm::vec4 color = glm::vec4(0.62f, 0.66f, 0.74f, 1.0f))
                : geometry::cube::cube(1.0f, 1.0f, 1.0f), object::objectBaseClass(collision, anchored, mass, velocity, position, rotation, scale, color) {}
    };
}
