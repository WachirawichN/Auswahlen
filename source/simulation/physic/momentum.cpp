#include "momentum.h"

float calculateAxisVelocity(float aAxisVelocity, float aMass, float bAxisVelocity, float bMass)
{
    return (((aMass - bMass) / (aMass + bMass)) * aAxisVelocity) + (((2 * bMass) / (aMass + bMass)) * bAxisVelocity);
}
glm::vec3 calculateNewObjectVelocity3D(glm::vec3 aVelocity, float aMass, glm::vec3 bVelocity, float bMass)
{
    std::vector<float> velocity;
    for (int axis = 0; axis < 3; axis++)
    {
        velocity.push_back(calculateAxisVelocity(aVelocity[axis], aMass, bVelocity[axis], bMass));
    }
    return glm::vec3(velocity[0], velocity[1], velocity[2]);
}

float momentum::elasticCollision1D(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target, unsigned int axis)
{
    float objectVelocity = object->getVelocity()[axis];
    if (object->isAnchored()) return objectVelocity;
    float objectMass = object->getMass();

    float targetVelocity = target->getVelocity()[axis];
    float targetMass = (!target->isAnchored()) ? target->getMass() : 999999999;

    float newObjVel = calculateAxisVelocity(objectVelocity, objectMass, targetVelocity, targetMass);
    return newObjVel;
}