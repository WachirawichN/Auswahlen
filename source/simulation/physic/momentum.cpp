#include "momentum.h"

float calculateAxisVelocity(float aAxisVelocity, float aMass, float bAxisVelocity, float bMass)
{
    return (((aMass - bMass) / (aMass + bMass)) * aAxisVelocity) + (((2 * bMass) / (aMass + bMass)) * bAxisVelocity);
}
glm::vec3 calculateObjectVelocity(glm::vec3 aVelocity, float aMass, glm::vec3 bVelocity, float bMass)
{
    std::vector<float> velocity;
    for (int axis = 0; axis < 3; axis++)
    {
        velocity.push_back(calculateAxisVelocity(aVelocity[axis], aMass, bVelocity[axis], bMass));
    }
    return glm::vec3(velocity[0], velocity[1], velocity[2]);
}

void momentum::elasticCollision(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target)
{
    glm::vec3 objectVelocity = object->getVelocity();
    float objectMass = object->getMass();

    glm::vec3 targetVelocity = target->getVelocity();
    float targetMass = target->getMass();

    object->changeVelocity(calculateObjectVelocity(objectVelocity, objectMass, targetVelocity, targetMass) - objectVelocity);
}