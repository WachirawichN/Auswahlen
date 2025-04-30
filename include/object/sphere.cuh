#pragma once

#include <GLC/matrix.cuh>
#include <GLC/vector.cuh>
#include <GLC/utility.cuh>

#include "object/objectBaseClass.cuh"
#include "geometry/icosphere.cuh"

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
                   GLC::vec3 velocity = GLC::vec3(0.0f),
                   GLC::vec3 position = GLC::vec3(0.0f),
                   GLC::vec3 rotation = GLC::vec3(0.0f),
                   GLC::vec3 scale = GLC::vec3(1.0f),
                   GLC::vec4 color = GLC::vec4(0.62f, 0.66f, 0.74f, 1.0f))
                : geometry::icosphere::icosphere(0.5f, subdivision), object::objectBaseClass(collision, anchored, mass, velocity, position, rotation, scale, color) {}
    };
}
