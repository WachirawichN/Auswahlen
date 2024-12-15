#include "collision.h"

enum collisionType {
    NO,
    NEWLY,
    ALREADY
};

float timeToMove(float aPosition, float bPosition, float aVelocity, float bVelocity)
{
    float totalVelocity = aVelocity - bVelocity;
    float dst = bPosition - aPosition;
    return (dst / totalVelocity);
}
std::vector<glm::vec3> borderPos(glm::vec3 objPos, glm::vec3 objScale, glm::vec3 tarPos, glm::vec3 tarScale)
{
    // Could use object and target's velocity for more accurate calculation
    // Need more optimization
    glm::vec3 objBorder = objPos;
    glm::vec3 tarBorder = tarPos;

    //std::cout << "   -  Inside or not" << std::endl;
    for (int axis = 0; axis < 3; axis++)
    {
        float objMaxBorder = objBorder[axis] + (objScale[axis] / 2.0f);
        float objMinBorder = objBorder[axis] - (objScale[axis] / 2.0f);

        float tarMaxBorder = tarBorder[axis] + (tarScale[axis] / 2.0f);
        float tarMinBorder = tarBorder[axis] - (tarScale[axis] / 2.0f);

        float side = objPos[axis] - tarPos[axis];

        // Check if target is inside object or vice versa
        if (tarPos[axis] < objMaxBorder && tarPos[axis] > objMinBorder)
        {
            // Target is inside object
            if (side > 0.0f)
            {
                objBorder[axis] = objMinBorder;
                tarBorder[axis] = tarMinBorder;
            }
            else
            {
                objBorder[axis] = objMaxBorder;
                tarBorder[axis] = tarMaxBorder;
            }
            //std::cout << "      -  Target is inside object" << std::endl;
            continue;
        }
        else if (objPos[axis] < tarMaxBorder && objPos[axis] > tarMinBorder)
        {
            // Object is inside target
            if (side > 0.0f)
            {
                objBorder[axis] = objMaxBorder;
                tarBorder[axis] = tarMaxBorder;
            }
            else
            {
                objBorder[axis] = objMinBorder;
                tarBorder[axis] = tarMinBorder;
            }
            //std::cout << "      -  Object is inside target" << std::endl;
            continue;
        }
        //std::cout << "      -  Not inside " << std::endl;

        // The object and the target is not within each other border
        if (side > 0)
        {
            // Object is on positive side compare to target position
            tarBorder[axis] = tarMaxBorder;

            // Calculate object border according to if the object is within target border or not
            float subSide = objPos[axis] - tarBorder[axis];
            if (subSide > 0)
            {
                objBorder[axis] = objMinBorder;
            }
            else if (subSide < 0)
            {
                objBorder[axis] = objMaxBorder;
            }
        }
        else if (side < 0)
        {
            // Object is on negative side compare to target position
            tarBorder[axis] = tarMinBorder;

            // Calculate object border according to if the object is within target border or not
            float subSide = objPos[axis] - tarBorder[axis];
            if (subSide > 0)
            {
                objBorder[axis] = objMinBorder;
            }
            else if (subSide < 0)
            {
                objBorder[axis] = objMaxBorder;
            }
        }
    }

    std::vector<glm::vec3> borders = {objBorder, tarBorder};
    return borders;
}

bool isInside(float objPos, float objScale, float tarPos)
{
    if ((tarPos < (objPos + objScale / 2.0f)) && (tarPos > (objPos - objScale / 2.0f))) return true;
    return false;
}
bool isSameDirection(float inputA, float inputB)
{
    if ((inputA < 0.0f && inputB < 0.0f) || (inputA > 0.0f && inputB > 0.0f) || (inputA == 0.0f && inputB == 0.0f)) return true;
    return false;
}
bool checkSide(float objPos, float tarPos)
{
    // True = Target is on the right
    // False = Target is on the left
    float side = objPos - tarPos;
    if (side < 0.0f) return true;
    return false;
}
std::vector<std::vector<float>> extendBorder(glm::vec3 objPos, glm::vec3 objScale)
{
    // Return format: {{+X, -X}, {+Y, -Y}, {+Z, -Z}}

    std::vector<std::vector<float>> borders;

    std::cout << "   -  Axis comparison: " << std::endl;
    for (int axis = 0; axis < 3; axis++)
    {
        std::vector<float> currentAxis;
        currentAxis.push_back(objPos[axis] + objScale[axis] / 2.0f);
        currentAxis.push_back(objPos[axis] - objScale[axis] / 2.0f);
        borders.push_back(currentAxis);
    }
    /*
    for (int axis = 0; axis < 3; axis++)
    {
        // Check if object is inside each other
        std::cout << "      -  Axis: " << axis << ", ";
        if (isInside(objPos[axis], objScale[axis], tarPos[axis]))
        {
            std::cout << "Target is inside object, ";
            // Target is inside the object
            if (isSameDirection(objVel[axis], tarVel[axis]))
            {
                std::cout << "Same direction, ";
                // Target and the object is moving along same direction
                int multiplier = 1;
                if (objVel[axis] > tarVel[axis])
                {
                    std::cout << "Object is faster than target" << std::endl;
                    // Extend border toward the opposite direction of the object and the target velocity
                    multiplier = (int)(objVel[axis] > 0.0f) * -2 + 1;
                    //multiplier = (objVel[axis] > 0.0f) ? -1 : 1;
                }
                else if (objVel[axis] < tarVel[axis])
                {
                    std::cout << "Target is faster than object" << std::endl;
                    // Extend border toward the same direction as the object and the target velocity
                    multiplier = (int)(objVel[axis] > 0.0f) * 2 + 1;
                    //multiplier = (objVel[axis] > 0.0f) ? 1 : -1;
                }
                else
                {
                    std::cout << "Both object is moving at the same speed" << std::endl;
                    multiplier = 0.0f;
                }
                objBorder[axis] += objScale[axis] / 2.0f * multiplier;
                tarBorder[axis] += tarScale[axis] / 2.0f * multiplier;
            }
            else
            {
                std::cout << "Opposite direction" << std::endl;
                int multiplier = 1;
                if (tarVel[axis] < 0.0f || objVel[axis] > 0.0f)
                {
                    multiplier = -1;
                }
                
                if (objVel[axis] == 0.0f)
                {
                    multiplier = (tarVel[axis] < 0.0f) ? -1 : 1;
                }
                else if (tarVel[axis] == 0.0f)
                {
                    multiplier = (objVel[axis] > 0.0f) ? -1 : 1;
                }
                else
                {
                    multiplier = (tarVel[axis] < 0.0f) ? -1 : 1;
                }
                
                // Calcuate the border base on the direction of the velocity of the target
                objBorder[axis] += objScale[axis] / 2.0f * multiplier;
                tarBorder[axis] += tarScale[axis] / 2.0f * multiplier;
            }
            continue;
        }
        else if (isInside(tarPos[axis], tarScale[axis], objPos[axis]))
        {
            std::cout << "Object is inside target, ";
            // Object is inside the target
            if (isSameDirection(objVel[axis], tarVel[axis]))
            {
                std::cout << "Same direction, ";
                // Target and the object is moving along same direction
                int multiplier = 1;
                if (tarVel[axis] > objVel[axis])
                {
                    std::cout << "Target is faster than object" << std::endl;
                    // Extend border toward the opposite direction of the target and the object velocity
                    multiplier = (objVel[axis] > 0.0f) ? -1 : 1;
                }
                else if (tarVel[axis] < objVel[axis])
                {
                    std::cout << "Object is faster than target" << std::endl;
                    // Extend border toward the same direction as the target and the object velocity
                    multiplier = (objVel[axis] > 0.0f) ? 1 : -1;
                }
                else
                {
                    std::cout << "Both object is moving at the same speed" << std::endl;
                    multiplier = 0.0f;
                }
                objBorder[axis] += objScale[axis] / 2.0f * multiplier;
                tarBorder[axis] += tarScale[axis] / 2.0f * multiplier;
            }
            else
            {
                std::cout << "Opposite direction" << std::endl;
                int multiplier = 1;
                if (tarVel[axis] > 0.0f || objVel[axis] < 0.0f)
                {
                    multiplier = -1;
                }
                
                if (objVel[axis] == 0.0f)
                {
                    multiplier = (tarVel[axis] > 0.0f) ? -1 : 1;
                }
                else if (tarVel[axis] == 0.0f)
                {
                    multiplier = (objVel[axis] < 0.0f) ? -1 : 1;
                }
                else
                {
                    multiplier = (objVel[axis] < 0.0f) ? -1 : 1;
                }
                
                // Calcuate the border base on the direction of the velocity of the object
                objBorder[axis] += objScale[axis] / 2.0f * multiplier;
                tarBorder[axis] += tarScale[axis] / 2.0f * multiplier;
            }
            continue;
        }

        bool side = checkSide(objPos[axis], tarPos[axis]);
        // Check if target is moving away from each other
        if (!isSameDirection(objVel[axis], tarVel[axis]))
        {
            std::cout << "Opposite dir" << std::endl;
            // Check if object will be able to hit each other or not
            if (side && (tarVel[axis] < 0.0f || objVel[axis] > 0.0f))
            {
                objBorder[axis] += objScale[axis] / 2.0f;
                tarBorder[axis] -= tarScale[axis] / 2.0f;
            }
            else if (!side && (tarVel[axis] > 0.0f || objVel[axis] < 0.0f))
            {
                objBorder[axis] -= objScale[axis] / 2.0f;
                tarBorder[axis] += tarScale[axis] / 2.0f;
            }
        }
        else if (side && objVel[axis] > tarVel[axis])
        {
            std::cout << "Same dir, object is faster" << std::endl;
            objBorder[axis] += objScale[axis] / 2.0f;
            tarBorder[axis] -= tarScale[axis] / 2.0f;
        }
        else if (!side && objVel[axis] < tarVel[axis])
        {
            std::cout << "Same dir, target is faster" << std::endl;
            objBorder[axis] -= objScale[axis] / 2.0f;
            tarBorder[axis] += tarScale[axis] / 2.0f;
        }
        else
        {
            std::cout << std::endl;
        }
    }

    std::vector<glm::vec3> borders;
    borders.push_back(objBorder);
    borders.push_back(tarBorder);
    */

    return borders;
}

std::vector<std::vector<unsigned int>> collision::collisionPairing(std::vector<std::shared_ptr<object::objectBaseClass>> objects)
{
    // The algorithm should sort the pairing base on the distance of each pair

    // Object at index 0 is the current object then after that is all the target object
    std::vector<std::vector<unsigned int>> pairs;

    for (unsigned int i = 0; i < objects.size(); i++)
    {
        std::shared_ptr<object::objectBaseClass> currentObject = objects.at(i);
        if (!currentObject->canCollide()) continue;
        std::vector<unsigned int> currentPair = {i};

        for (unsigned int j = i + 1; j < objects.size(); j++)
        {
            std::shared_ptr<object::objectBaseClass> targetObject = objects.at(j);
            if (!targetObject->canCollide()) continue;
            currentPair.push_back(j);
        }

        if (currentPair.size() >= 1)
        {
            pairs.push_back(currentPair);
        }
    }

    return pairs;
}
glm::vec3 collision::dstBaseCD(std::shared_ptr<object::objectBaseClass> object, std::shared_ptr<object::objectBaseClass> target, glm::vec3 deltaTime)
{
    glm::vec3 objPos = object->getPosition();
    glm::vec3 objScale = object->getScale();
    glm::vec3 objVel = object->getVelocity();

    glm::vec3 tarPos = target->getPosition();
    glm::vec3 tarScale = target->getScale();
    glm::vec3 tarVel = target->getVelocity();

    // Stretch out border of object and target according to what side of object is correlate to target on all axis
   std::vector<glm::vec3> borders = borderPos(objPos, objScale, tarPos, tarScale);
   glm::vec3 objBorder = borders[0];
   glm::vec3 tarBorder = borders[1];


   //std::vector<std::vector<float>> testObjBor = extendBorder(objPos, objScale);
   //std::vector<std::vector<float>> testTarObj = extendBorder(tarPos, tarScale);

   std::cout << "   -  Current object: " << std::endl;
   std::cout << "      -  Position: " << objPos.x << ", " << objPos.y << ", " << objPos.z << std::endl;
   std::cout << "      -  Border: " << objBorder.x << ", " << objBorder.y << ", " << objBorder.z << std::endl;
   std::cout << "      -  Velocity: " << objVel.x << ", " << objVel.y << ", " << objVel.z << std::endl;

   std::cout << "   -  Target object: " << std::endl;
   std::cout << "      -  Position: " << tarPos.x << ", " << tarPos.y << ", " << tarPos.z << std::endl;
   std::cout << "      -  Border: " << tarBorder.x << ", " << tarBorder.y << ", " << tarBorder.z << std::endl;
   std::cout << "      -  Velocity: " << tarVel.x << ", " << tarVel.y << ", " << tarVel.z << std::endl;

    // Actual collision detection on each axis
    unsigned int collideAxis = 0;
    unsigned int maxCollideAxis = 3;
    glm::vec3 travelTimes(0.0f, 0.0f, 0.0f);
    std::cout << "   -  Colliding stat: " << std::endl;
    for (int axis = 0; axis < 3; axis++)
    {
        std::cout << "      -  Axis: " << axis << std::endl;
        bool isTargetInside = isInside(objPos[axis], objScale[axis], tarPos[axis]);
        bool isObjectInside = isInside(tarPos[axis], tarScale[axis], objPos[axis]);
        bool side = checkSide(objPos[axis], tarPos[axis]);

        glm::vec2 objAxisBorder;
        glm::vec2 tarAxisBorder;

        if (isTargetInside && isObjectInside)
        {
            std::cout << "         -  Object is inside each other" << std::endl;
        }
        else if (isTargetInside)
        {
            std::cout << "         -  Target is inside object" << std::endl;
        }
        else if (isObjectInside)
        {
            std::cout << "         -  Object is inside target" << std::endl;
        }
        else
        {
            std::cout << "         -  Object is not inside target nor the target does" << std::endl;
        }
        

        // Need more improvement on if else logic
        // Prevent from diving by zero
        if (objVel[axis] == tarVel[axis])
        {
            if (tarBorder[axis] == objBorder[axis])
            {
                collideAxis++;
                std::cout << "         -  Collided, equal vel equal border" << std::endl;
            }
            continue;
        }

        // If the travelTime is 0 then both border of the object and the target is on the same spot
        float travelTime = mathExt::roundToDec(timeToMove(objBorder[axis], tarBorder[axis], objVel[axis], tarVel[axis]), 5);
        if ((deltaTime[axis] > 0.0f && travelTime <= deltaTime[axis] && travelTime > 0.0f) || (deltaTime[axis] < 0.0f && travelTime >= deltaTime[axis] && travelTime < 0.0f) || travelTime == 0.0f)
        {
            collideAxis++;
            travelTimes[axis] = travelTime;
            std::cout << "         -  Collided, delta time condition, delta time: " << deltaTime[axis] << ", travelTime: " << travelTime << std::endl;
        }
        else if ((deltaTime[axis] > 0.0f && travelTime < 0.0f) || (deltaTime[axis] < 0.0f && travelTime > 0.0f))
        {
            maxCollideAxis--;
            std::cout << "         -  Reduced, delta time: " << deltaTime[axis] << ", travelTime: " << travelTime << std::endl;
        }
        else
        {
            std::cout << "         -  Not collided, delta time: " << deltaTime[axis] << ", travelTime: " << travelTime << std::endl;
        }
    }

    // Collision have been detected
    if (collideAxis == maxCollideAxis)
    {
        return travelTimes;
        std::cout << "   -  Collided" << std::endl;
    }
    std::cout << "   -  Not collided" << std::endl;
    
    // Object did collide with target
    return glm::vec3(0.0f);
}
void collision::collsionResolver(std::shared_ptr<object::objectBaseClass> currentObject, std::shared_ptr<object::objectBaseClass> target, glm::vec3 travelTime)
{
    // Move to collision site
    currentObject->move(fundamental::calculateDstVecT(currentObject->getVelocity(), travelTime));
    target->move(fundamental::calculateDstVecT(target->getVelocity(), travelTime));

    // Update new velocity
    glm::vec3 objNewVel = momentum::elasticCollision(currentObject, target);
    glm::vec3 tarNewVel = momentum::elasticCollision(target, currentObject);
    currentObject->changeVelocity(objNewVel - currentObject->getVelocity());
    target->changeVelocity(tarNewVel - target->getVelocity());
}
