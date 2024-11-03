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

            glm::vec3 position;
            glm::vec3 rotation;
            glm::vec3 scale;
        public:
            objectBaseClass(bool collision = true, bool gravity = true, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f))
                : collision(collision), gravity(gravity), position(position), rotation(rotation), scale(scale) {}

            void setCollision(bool newValue) {collision = newValue;}
            void setGravity(bool newValue) {gravity = newValue;}

            void move(glm::vec3 newPosition) {position = newPosition;}
            void rotate(glm::vec3 newDegree) {rotation = newDegree;}
            void rescale(glm::vec3 newScale) {scale = newScale;}

            virtual glm::vec3 getPosition() const {return position;}
            virtual glm::vec3 getRotation() const {return rotation;}
            virtual glm::vec3 getScale() const {return scale;}
    };
}