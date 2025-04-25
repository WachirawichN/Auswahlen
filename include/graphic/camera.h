#pragma once

#include "../dependencies/GLM/glm.hpp"
#include "../dependencies/GLM/ext.hpp"

// Free flying camera system
class camera
{
    private:
        // Coordinates
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;

        // Projection
        float fov;
        float aspectRatio;
        float near;
        float far;

        float yaw = -90.0f;
        float pitch = 0.0f;

        glm::mat4 view;
        glm::mat4 projection;
    public:
        camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
               float fov = float(90.0f), float aspectRatio = (float)16/(float)9, float near = float(0.1f), float far = float(100.0f));
               
        glm::mat4 getView() const;
        glm::mat4 getProjection() const;

        void rotateCamera(float deltaYaw, float deltaPitch);

        void move(float speed, float deltaTime, bool sideway = false, bool invert = false);
        void teleportTo(glm::vec3 newPosition);
        void changeFov(float newFlov);
};