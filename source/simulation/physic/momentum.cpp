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
    float objectMass = object->getMass();
    if (object->isAnchored()) objectMass = 999999999;

    float targetVelocity = target->getVelocity()[axis];
    float targetMass = target->getMass();
    if (target->isAnchored()) targetMass = 999999999;

    float newObjVel = calculateAxisVelocity(objectVelocity, objectMass, targetVelocity, targetMass);
    return newObjVel;
}
glm::vec3 momentum::elasticCollision3D(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target)
{
    glm::vec3 objectVelocity = object->getVelocity();
    float objectMass = object->getMass();
    if (object->isAnchored()) objectMass = 999999999;

    glm::vec3 targetVelocity = target->getVelocity();
    float targetMass = target->getMass();
    if (target->isAnchored()) targetMass = 999999999;

    glm::vec3 newObjVel = calculateNewObjectVelocity3D(objectVelocity, objectMass, targetVelocity, targetMass);
    return newObjVel;
}