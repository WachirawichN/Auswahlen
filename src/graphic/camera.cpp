#include "graphic/camera.h"

camera::camera(GLC::vec3 position, GLC::vec3 up,
               float fov, float aspectRatio, float near, float far)
    :position(position), front(GLC::vec3(0.0f, 0.0f, -1.0f)), up(up),
     fov(fov), aspectRatio(aspectRatio), near(near), far(far)
{
    view = GLC::lookAt(position, position + front, up);
    projection = GLC::perspective(fov, aspectRatio, near, far);
}


GLC::mat4 camera::getView() const
{
    return view;
}

GLC::mat4 camera::getProjection() const
{
    return projection;
}


void camera::rotateCamera(float deltaYaw, float deltaPitch)
{
    yaw += deltaYaw;
    pitch += deltaPitch;
    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    else if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    GLC::vec3 direction;
    direction[0] = cos(GLC::radians(yaw)) * cos(GLC::radians(pitch));
    direction[1] = sin(GLC::radians(pitch));
    direction[2] = sin(GLC::radians(yaw)) * cos(GLC::radians(pitch));
    front = GLC::normalize(direction);
    view = GLC::lookAt(position, position + front, up);
}


void camera::move(float speed, float deltaTime, bool sideway, bool invert)
{
    GLC::vec3 distance;
    if (sideway)
    {
        distance = GLC::normalize(GLC::cross(front, up)) * (speed * deltaTime * (invert * -2 + 1));
    }
    else
    {
        distance = front * (speed * deltaTime * (invert * -2 + 1));
    }
    position += distance;
    view = GLC::lookAt(position, position + front, up);
}

void camera::teleportTo(GLC::vec3 newPosition)
{
    position = newPosition;
    view = GLC::lookAt(position, position + front, up);
}

void camera::changeFov(float newFov)
{
    fov = newFov;
    projection = GLC::perspective(fov, aspectRatio, near, far);
}