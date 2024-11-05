#pragma once

#include "../../dependencies/GLM/glm.hpp"
#include "../../graphic/geometry/geometry.h"

namespace object
{
    class objectBaseClass
    {
        protected:
            bool collision;
            bool gravity;

            float mass;
            glm::vec3 velocity;

            glm::vec3 position;
            glm::vec3 rotation;
            glm::vec3 scale;
        public:
            objectBaseClass(bool collision = true,
                            bool gravity = true,
                            float mass = 1,
                            glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
            virtual ~objectBaseClass() = default;

            bool isCollide();
            bool isGravityAffected();

            void setCollision(bool newValue);
            void setGravity(bool newValue);

            void changeVelocity(glm::vec3 deltaVelocity);
            void move(glm::vec3 distance);
            void rotate(glm::vec3 degree);
            void rescale(glm::vec3 deltaScale);

            glm::vec3 getVelocity() const;
            glm::vec3 getPosition() const;
            glm::vec3 getRotation() const;
            glm::vec3 getScale() const;
    };
}