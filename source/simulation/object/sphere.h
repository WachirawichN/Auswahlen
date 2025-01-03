#pragma once

#include "objectBaseClass.cuh"
#include "../../graphic/geometry/icosphere.h"

namespace object
{
    class sphere : public geometry::icosphere, public object::objectBaseClass
    {
        private:
        public:
            sphere(unsigned int subdivision = 1,
                   bool collision = true,
                   bool anchored = false,
                   float mass = 1,
                   glm::vec3 velocity = glm::vec3(0.0f),
                   glm::vec3 position = glm::vec3(0.0f),
                   glm::vec3 rotation = glm::vec3(0.0f),
                   glm::vec3 scale = glm::vec3(1.0f),
                   glm::vec4 color = glm::vec4(0.62f, 0.66f, 0.74f, 1.0f))
                : geometry::icosphere::icosphere(0.5f, subdivision), object::objectBaseClass(collision, anchored, mass, velocity, position, rotation, scale, color) {}
    };
}
