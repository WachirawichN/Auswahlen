#pragma once

#include <cuda_runtime.h>

#include "GLC/matrix.cuh"
#include "GLC/vector.cuh"
#include "GLC/utility.cuh"

// Free flying camera system
class camera
{
    private:
        // Coordinates
        GLC::vec3 position;
        GLC::vec3 front;
        GLC::vec3 up;

        // Projection
        float fov;
        float aspectRatio;
        float near;
        float far;

        float yaw = -90.0f;
        float pitch = 0.0f;

        GLC::mat4 view;
        GLC::mat4 projection;
    public:
        camera(GLC::vec3 position = GLC::vec3(0.0f, 0.0f, 0.0f), GLC::vec3 up = GLC::vec3(0.0f, 1.0f, 0.0f),
               float fov = float(90.0f), float aspectRatio = (float)16/(float)9, float near = float(0.1f), float far = float(100.0f));
               
        GLC::mat4 getView() const;
        GLC::mat4 getProjection() const;

        void rotateCamera(float deltaYaw, float deltaPitch);

        void move(float speed, float deltaTime, bool sideway = false, bool invert = false);
        void teleportTo(GLC::vec3 newPosition);
        void changeFov(float newFlov);
};