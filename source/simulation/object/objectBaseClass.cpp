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

object::objectBaseClass::objectBaseClass(bool collision, bool gravity, glm::vec3 position, glm::vec3 inRotation, glm::vec3 scale)
    : collision(collision), gravity(gravity), position(position), rotation(rotation), scale(scale)
{
    // Make the rotation be within 360 degree
    rotation = normalizeRotation(rotation);
    std::cout << "Set rotation: " << getRotation().x << ", " << getRotation().y << ", " << getRotation().z << std::endl;
}

bool object::objectBaseClass::isCollide()
{
    return collision;
}

bool object::objectBaseClass::isGravityAffected()
{
    return collision;
}

void object::objectBaseClass::setCollision(bool newValue)
{
    collision = newValue;
}
void object::objectBaseClass::setGravity(bool newValue)
{
    gravity = newValue;
}

void object::objectBaseClass::move(glm::vec3 distnace)
{
    position += distnace;
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