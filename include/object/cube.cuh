#pragma once

#include <GLC/matrix.cuh>
#include <GLC/vector.cuh>
#include <GLC/utility.cuh>

#include "object/objectBaseClass.cuh"
#include "geometry/cube.cuh"

namespace object
{
    class cube : public geometry::cube, public object::objectBaseClass
    {
        private:
        public:
            cube(bool collision = true,
                 bool anchored = false,
                 float mass = 1,
                 GLC::vec3 velocity = GLC::vec3(0.0f),
                 GLC::vec3 position = GLC::vec3(0.0f),
                 GLC::vec3 rotation = GLC::vec3(0.0f),
                 GLC::vec3 scale = GLC::vec3(1.0f),
                 GLC::vec4 color = GLC::vec4(0.62f, 0.66f, 0.74f, 1.0f))
                : geometry::cube::cube(1.0f, 1.0f, 1.0f), object::objectBaseClass(collision, anchored, mass, velocity, position, rotation, scale, color) {}
    };
}
