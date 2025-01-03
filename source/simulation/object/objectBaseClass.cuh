#pragma once

#include "../../dependencies/GLM/glm.hpp"
#include "../../graphic/geometry/geometry.h"

namespace object
{
    class objectBaseClass
    {
        protected:
            bool collision;
            bool anchored;

            float mass;
            glm::vec3 velocity;
            float collisionTime;

            glm::vec3 position;
            glm::vec3 rotation;
            glm::vec3 scale;

            glm::vec4 color;
        public:
            __host__ objectBaseClass(bool collision = true,
                                     bool anchored = false,
                                     float mass = 1,
                                     glm::vec3 velocity = glm::vec3(0.0f),
                                     glm::vec3 position = glm::vec3(0.0f),
                                     glm::vec3 rotation = glm::vec3(0.0f),
                                     glm::vec3 scale = glm::vec3(1.0f),
                                     glm::vec4 color = glm::vec4(0.62f, 0.66f, 0.74f, 1.0f));
            __host__ virtual ~objectBaseClass() = default;

            __host__ __device__ bool canCollide();
            __host__ __device__ bool isAnchored();

            void setCollision(bool newValue);
            void setAnchored(bool newValue);

            __host__ __device__ void changeVelocity(glm::vec3 deltaVelocity);
            __host__ __device__ void changeCollisionTime(float deltaTime);

            __host__ __device__ void move(glm::vec3 distance);
            __host__ __device__ void rotate(glm::vec3 degree);
            __host__ __device__ void rescale(glm::vec3 deltaScale);

            void setColor(glm::vec4 newColor);

            __host__ __device__ float getMass() const;
            __host__ __device__ glm::vec3 getVelocity() const;
            __host__ __device__ float getCollisionTime() const;

            __host__ __device__ glm::vec3 getPosition() const;
            __host__ __device__ glm::vec3 getRotation() const;
            __host__ __device__ glm::vec3 getScale() const;

            glm::vec4 getColor() const;
    };
}