#include "camera.h"

camera::camera(glm::vec3 position, glm::vec3 up,
               float fov, float aspectRatio, float near, float far)
    :position(position), front(glm::vec3(0.0f, 0.0f, -1.0f)), up(up),
     fov(fov), aspectRatio(aspectRatio), near(near), far(far)
{
    view = glm::lookAt(position, position + front, up);
    projection = glm::perspective(fov, aspectRatio, near, far);
}


glm::mat4 camera::getView() const
{
    return view;
}

glm::mat4 camera::getProjection() const
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

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
    view = glm::lookAt(position, position + front, up);
}


void camera::move(float speed, float deltaTime, bool sideway, bool invert)
{
    glm::vec3 distance;
    if (sideway)
    {
        distance = glm::normalize(glm::cross(front, up)) * (speed * deltaTime * (invert * -2 + 1));
    }
    else
    {
        distance = front * (speed * deltaTime * (invert * -2 + 1));
    }
    position += distance;
    view = glm::lookAt(position, position + front, up);
}

void camera::teleportTo(glm::vec3 newPosition)
{
    position = newPosition;
    view = glm::lookAt(position, position + front, up);
}

void camera::changeFov(float newFov)
{
    fov = newFov;
    projection = glm::perspective(fov, aspectRatio, near, far);
}