#include "collision.cuh"

float timeToMove(float aPosition, float aVelocity, float bPosition, float bVelocity)
{
    float totalVelocity = aVelocity - bVelocity;
    float dst = bPosition - aPosition;
    return (totalVelocity != 0.0f) ? (dst / totalVelocity) : 999999.99f;
}
std::vector<float> unbalanceTimeToMove(float aPosition, float aVelocity, float aRemainTime, float bPosition, float bVelocity, float bRemainTime)
{
    unsigned int lowestTime = (aRemainTime < bRemainTime) ? 0 : 1;
    return std::vector<float>({0.0f});
}

bool isInside(std::vector<float> aBorders, std::vector<float> bBorders)
{
    bool positiveSide = (bBorders[2] < aBorders[2]) && (bBorders[0] < aBorders[2]);
    bool negativeSide = (bBorders[2] > aBorders[0]) && (bBorders[0] > aBorders[0]);

    return (positiveSide && negativeSide);
}
bool isCross(std::vector<float> aBorders, std::vector<float> bBorders)
{
    bool positiveSide = ((bBorders[0] < aBorders[2]) && !(bBorders[2] < aBorders[2])) || (!(bBorders[0] < aBorders[2]) && (bBorders[2] < aBorders[2]));
    bool negativeSide = ((bBorders[2] < aBorders[0]) && !(bBorders[0] < aBorders[0])) || (!(bBorders[2] < aBorders[0]) && (bBorders[0] < aBorders[0]));

    return (positiveSide || negativeSide);
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
std::vector<collision::collisionType> collision::CCD(std::shared_ptr<object::objectBaseClass> obj, std::shared_ptr<object::objectBaseClass> tar, float deltaTime)
{
    std::vector<collision::collisionType> collisionResults;
    int timeMultiplier = (deltaTime > 0.0f) ? 1 : -1;

    glm::vec3 objPos = obj->getPosition();
    glm::vec3 objScale = obj->getScale();
    glm::vec3 objVel = obj->getVelocity();
    float objectDeltaTime = obj->getCollisionTime();

    glm::vec3 tarPos = tar->getPosition();
    glm::vec3 tarScale = tar->getScale();
    glm::vec3 tarVel = tar->getVelocity();
    float targetDeltaTime = obj->getCollisionTime();

    std::cout << "   -  Selected border:" << std::endl;
    for (int axis = 0; axis < 3; axis++)
    {
        // Calculate the border of both object
        std::vector<float> objBorders = hitbox::cubeHitbox(objPos[axis], objScale[axis]);
        std::vector<float> tarBorders = hitbox::cubeHitbox(tarPos[axis], tarScale[axis]);

        // Check if the object is inside each other or not
        bool tarInObj = isInside(objBorders, tarBorders);
        bool objInTar = isInside(tarBorders, objBorders);


        // Choose object border
        std::vector<int> selectedBorder = hitbox::hitboxSide(objPos[axis], objVel[axis], tarPos[axis], tarVel[axis], tarInObj, objInTar, timeMultiplier);
        int objBorderIndex = selectedBorder[0] + 1;
        int tarBorderIndex = selectedBorder[1] + 1;

        float objBorder = objBorders[objBorderIndex];
        float tarBorder = tarBorders[tarBorderIndex];
        std::cout << "      -  Axis: " << axis << ", object's: " << objBorderIndex << ", target's: " << tarBorderIndex << std::endl;

        // Check for NEWLY collision type
        float travelTime = timeToMove(objBorder, objVel[axis], tarBorder, tarVel[axis]);
        if (abs(deltaTime) > 0.0f && mathExt::roundToDec(abs(travelTime), 6) > 0.0f && abs(deltaTime) > abs(travelTime))
        {
            if (mathExt::direction(deltaTime) == mathExt::direction(travelTime))
            {
                //std::cout << "Travel time: " << mathExt::roundToDec(travelTime, 6) << ", " << (abs(travelTime) > 0.0f) << std::endl;
                collisionResults.push_back(collision::collisionType::NEWLY);
                std::cout << "         -  Distance: " << tarBorder - objBorder << ", Velocity: " << objVel[axis] - tarVel[axis] << std::endl;
                continue;
            }
        }

        // Check for CROSS and INSIDE colision type
        if (objInTar || tarInObj)
        {
            collisionResults.push_back(collision::collisionType::INSIDE);
            continue;
        }
        else if (isCross(objBorders, tarBorders))
        {
            collisionResults.push_back(collision::collisionType::CROSS);
            continue;
        }

        // No collision has been occure
        collisionResults.push_back(collision::collisionType::NO);
    }

    return collisionResults;
}
float collision::collisionResolver(std::shared_ptr<object::objectBaseClass> obj, std::shared_ptr<object::objectBaseClass> tar, float deltaTime, std::vector<unsigned int> newlyAxis)
{
    // Working instruction:
    // -  Step 1:
    //    -  Find the time that two object need to use to travel to collision site in all axis that is NEWLY collide axis.
    // -  Step 2:
    //    -  Put those travel times through pythagoras to get final time to move before collision will be occured.
    // -  Step 3:
    //    -  Move those object to collision site.
    // -  Step 4:
    //    -  Update object and target velocity that is NEWLY collide axis.

    int timeMultiplier = (deltaTime > 0.0f) ? 1 : -1;

    glm::vec3 objPos = obj->getPosition();
    glm::vec3 objScale = obj->getScale();
    glm::vec3 objVel = obj->getVelocity();

    glm::vec3 tarPos = tar->getPosition();
    glm::vec3 tarScale = tar->getScale();
    glm::vec3 tarVel = tar->getVelocity();

    // Step 1.
    std::vector<float> travelTimes;
    for (unsigned int axis : newlyAxis)
    {
        // Calculate the border of both object
        std::vector<float> objBorders = hitbox::cubeHitbox(objPos[axis], objScale[axis]);
        std::vector<float> tarBorders = hitbox::cubeHitbox(tarPos[axis], tarScale[axis]);

        // Check if the object is inside each other or not
        bool tarInObj = isInside(objBorders, tarBorders);
        bool objInTar = isInside(tarBorders, objBorders);

        // Choose object border
        std::vector<int> selectedBorder = hitbox::hitboxSide(objPos[axis], objVel[axis], tarPos[axis], tarVel[axis], tarInObj, objInTar, timeMultiplier);
        int objMultiplier = selectedBorder[0] + 1;
        int tarMultiplier = selectedBorder[1] + 1;

        float objBorder = objBorders[objMultiplier];
        float tarBorder = tarBorders[tarMultiplier];

        float travelTime = timeToMove(objBorder, objVel[axis], tarBorder, tarVel[axis]);
        travelTimes.push_back(travelTime);
    }

    // Step 2.
    float lowestTime = *std::min_element(travelTimes.begin(), travelTimes.end());

    // Step 3.
    obj->move(fundamental::calculateDst(objVel, lowestTime));
    tar->move(fundamental::calculateDst(tarVel, lowestTime));

    // Step 4.
    glm::vec3 objDeltaVel(0.0f);
    glm::vec3 tarDeltaVel(0.0f);
    std::cout << "      -  Pythagoras time: " << lowestTime << std::endl;
    std::cout << "      -  Resolving collision" << std::endl;
    for (unsigned int axis : newlyAxis)
    {
        float objNewVel = momentum::elasticCollision1D(obj, tar, axis);
        float tarNewVel = momentum::elasticCollision1D(tar, obj, axis);

        tarDeltaVel[axis] = tarNewVel - tarVel[axis];
        objDeltaVel[axis] = objNewVel - objVel[axis];

        std::cout << "         -  Axis: " << axis << std::endl;
        std::cout << "            -  Axis travel time: " << travelTimes[axis] << std::endl;
        std::cout << "            -  Object original axis vel: " << objVel[axis] << ", new axis vel: " << objNewVel << std::endl;
        std::cout << "            -  Target original axis vel: " << tarVel[axis] << ", new axis vel: " << tarNewVel << std::endl;
        std::cout << "            -  Object original axis pos: " << objPos[axis] << ", new axis pos: " << obj->getPosition()[axis] << std::endl;
        std::cout << "            -  Target original axis pos: " << tarPos[axis] << ", new axis pos: " << tar->getPosition()[axis] << std::endl;
    }
    obj->changeVelocity(objDeltaVel);
    tar->changeVelocity(tarDeltaVel);

    return lowestTime;
}
