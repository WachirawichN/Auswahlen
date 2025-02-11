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

object::objectBaseClass::objectBaseClass(bool collision, bool anchored, float mass, glm::vec3 velocity, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec4 color)
    : collision(collision), anchored(anchored), mass(mass), velocity(velocity), collisionTime(float(0.0f)), position(position), rotation(normalizeRotation(rotation)), scale(scale), color(color)
{
}

bool object::objectBaseClass::canCollide()
{
    return collision;
}
bool object::objectBaseClass::isAnchored()
{
    return anchored;
}

void object::objectBaseClass::setCollision(bool newValue)
{
    collision = newValue;
}
void object::objectBaseClass::setAnchored(bool newValue)
{
    anchored = newValue;
}

void object::objectBaseClass::changeVelocity(glm::vec3 deltaVelocity)
{
    velocity += deltaVelocity;
}
void object::objectBaseClass::changeCollisionTime(float deltaTime)
{
    collisionTime += deltaTime;
}

void object::objectBaseClass::move(glm::vec3 distance)
{
    position += distance;
}
void object::objectBaseClass::rotate(glm::vec3 degree)
{
    rotation += degree;
    rotation = normalizeRotation(rotation); // Make the rotation be within 360 degree
}
void object::objectBaseClass::rescale(glm::vec3 deltaScale)
{
    scale = deltaScale;
}

void object::objectBaseClass::setColor(glm::vec4 newColor)
{
    color = newColor;
}

float object::objectBaseClass::getMass() const
{
    return mass;
}
glm::vec3 object::objectBaseClass::getVelocity() const
{
    return velocity;
}
float object::objectBaseClass::getCollisionTime() const
{
    return collisionTime;
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

glm::vec4 object::objectBaseClass::getColor() const
{
    return color;
}