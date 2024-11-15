#include "objectBaseClass.h"

#include <iostream>

glm::vec3 normalizeRotation(glm::vec3 objectRotation)
{
    glm::vec3 newRotation(objectRotation.x, objectRotation.y, objectRotation.z);
    for (int i = 0; i < 3; i++)
    {
        if (objectRotation[i] >= 360 || objectRotation[i] < 0)
        {
            int multiplier = floor(objectRotation[i] / 360.0f);
            newRotation[i] -= multiplier * 360.0f;
        }
    }
    return newRotation;
}

object::objectBaseClass::objectBaseClass(bool collision, bool gravity, float mass, glm::vec3 velocity, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : collision(collision), gravity(gravity), mass(mass), velocity(velocity), position(position), rotation(normalizeRotation(rotation)), scale(scale)
{
    //std::cout << gravity << std::endl;
    //std::cout << "Set rotation: " << getRotation().x << ", " << getRotation().y << ", " << getRotation().z << std::endl;
}

bool object::objectBaseClass::isCollide()
{
    return collision;
}
bool object::objectBaseClass::isGravityAffected()
{
    return gravity;
}

void object::objectBaseClass::setCollision(bool newValue)
{
    collision = newValue;
}
void object::objectBaseClass::setGravity(bool newValue)
{
    gravity = newValue;
}

void object::objectBaseClass::changeVelocity(glm::vec3 deltaVelocity)
{
    velocity += deltaVelocity;
}
void object::objectBaseClass::move(glm::vec3 distance)
{
    position += distance;
}
void object::objectBaseClass::rotate(glm::vec3 degree)
{
    rotation += degree;
    rotation = normalizeRotation(rotation); // Make the rotation be within 360 degree
    //std::cout << "New rotation: " << getRotation().x << ", " << getRotation().y << ", " << getRotation().z << std::endl;
}
void object::objectBaseClass::rescale(glm::vec3 deltaScale)
{
    scale = deltaScale;
}

glm::vec3 object::objectBaseClass::getVelocity() const
{
    return velocity;
}
glm::vec3 object::objectBaseClass::getPosition() const
{
    return position;
}
glm::vec3 object::objectBaseClass::getRotation() const
{
    return rotation;
}
glm::vec3 object::objectBaseClass::getScale() const
{
    return scale;
}