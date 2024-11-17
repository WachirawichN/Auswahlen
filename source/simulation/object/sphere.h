#pragma once

#include "objectBaseClass.h"
#include "../../graphic/geometry/icosphere.h"

namespace object
{
    class sphere : public geometry::icosphere, public object::objectBaseClass
    {
        private:
        public:
            sphere(int subdivision = 1,
                   bool collision = true,
                   bool anchored = false,
                   float mass = 1,
                   glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f))
                : geometry::icosphere::icosphere(0.5f, subdivision), object::objectBaseClass(collision, anchored, mass, velocity, position, rotation, scale) {}
    };
}
