#pragma once

#include "objectBaseClass.h"
#include "../../graphic/geometry/icosphere.h"

namespace object
{
    class sphere : public geometry::icosphere, public object::objectBaseClass
    {
        private:
        public:
            sphere(float radius, int subdivision = 1, bool collision = true, bool gravity = true, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f))
                : geometry::icosphere::icosphere(radius, subdivision), object::objectBaseClass(collision, gravity, position, rotation, scale) {}
    };
}
