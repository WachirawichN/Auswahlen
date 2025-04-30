#pragma once

#include <GLC/matrix.cuh>
#include <GLC/vector.cuh>
#include <GLC/utility.cuh>

#include "geometry/geometry.cuh"

namespace object
{
    class objectBaseClass
    {
        protected:
            bool collision;
            bool anchored;

            float mass;
            GLC::vec3 velocity;
            float collisionTime;

            GLC::vec3 position;
            GLC::vec3 rotation;
            GLC::vec3 scale;

            GLC::vec4 color;
        public:
            __host__ objectBaseClass(bool collision = true,
                                     bool anchored = false,
                                     float mass = 1,
                                     GLC::vec3 velocity = GLC::vec3(0.0f),
                                     GLC::vec3 position = GLC::vec3(0.0f),
                                     GLC::vec3 rotation = GLC::vec3(0.0f),
                                     GLC::vec3 scale = GLC::vec3(1.0f),
                                     GLC::vec4 color = GLC::vec4(0.62f, 0.66f, 0.74f, 1.0f));
            __host__ virtual ~objectBaseClass() = default;

            __host__ __device__ bool canCollide();
            __host__ __device__ bool isAnchored();

            void setCollision(bool newValue);
            void setAnchored(bool newValue);

            __host__ __device__ void changeVelocity(GLC::vec3 deltaVelocity);
            __host__ __device__ void changeCollisionTime(float deltaTime);

            __host__ __device__ void move(GLC::vec3 distance);
            __host__ __device__ void rotate(GLC::vec3 degree);
            __host__ __device__ void rescale(GLC::vec3 deltaScale);

            void setColor(GLC::vec4 newColor);

            __host__ __device__ float getMass() const;
            __host__ __device__ GLC::vec3 getVelocity() const;
            __host__ __device__ float getCollisionTime() const;

            __host__ __device__ GLC::vec3 getPosition() const;
            __host__ __device__ GLC::vec3 getRotation() const;
            __host__ __device__ GLC::vec3 getScale() const;

            GLC::vec4 getColor() const;
    };
}