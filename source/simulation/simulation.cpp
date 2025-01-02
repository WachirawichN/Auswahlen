#include "simulation.h"

simulation::simulation(camera* mainCamera, shader programShader, float gravity)
    : mainCamera(mainCamera), programShader(programShader), gravity(gravity)
{
    programShader.setUniformMat4fv("projection", GL_FALSE, mainCamera->getProjection());
}

void simulation::addObject(std::shared_ptr<object::objectBaseClass> newObject)
{
    objects.push_back(newObject);
}
void simulation::drawSimulation()
{
    workspaceRenderer.clearScreen();
    programShader.setUniformMat4fv("view", GL_FALSE, mainCamera->getView()); // Should only change when camera move

    for (int i = 0; i < objects.size(); i++)
    {
        std::shared_ptr<object::objectBaseClass> currentObject = objects.at(i);

        geometry::shape* shapePtr = dynamic_cast<geometry::shape*>(currentObject.get());
        if (shapePtr) // Check if object is inheritance of shape class
        {
            std::vector<float> vertices = shapePtr->getVertices();
            std::vector<unsigned int> indices = shapePtr->getIndices();

            // Could be more efficient if make the object class have its own buffer but may use more ram
            vertexArray va;
            vertexBuffer vb(vertices.size(), vertices.data());
            vertexBufferLayout layout;
            layout.addLayout(3);
            layout.addLayout(2);
            indexBuffer ib(indices.size(), indices.data());
            va.addBuffer(vb, layout);

            glm::mat4 modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, currentObject->getPosition());
            modelMatrix = glm::scale(modelMatrix, currentObject->getScale());

            glm::vec3 objectRotation = currentObject->getRotation();
            float degree = std::max(objectRotation.x, std::max(objectRotation.y, objectRotation.z));
            if (degree != 0.0f)
            {
                modelMatrix = glm::rotate(modelMatrix, glm::radians(degree), glm::vec3(objectRotation.x / degree, objectRotation.y / degree, objectRotation.z / degree));
            }

            programShader.setUniformMat4fv("model", GL_FALSE, modelMatrix);
            workspaceRenderer.drawScreen(va, ib, programShader);

            va.unbind();
            vb.unbind();
            ib.unbind();
        }
    }
    
}
void simulation::updateSimulation(float deltaTime)
{
    // Apply gravity
    for (int i = 0; i < objects.size(); i++)
    {
        std::shared_ptr<object::objectBaseClass> currentObject = objects.at(i);
        
        if (!currentObject->isAnchored())
        {
            glm::vec3 deltaVelocity = fundamental::calculateVel(glm::vec3(0.0f, gravity, 0.0f), deltaTime);
            currentObject->changeVelocity(deltaVelocity);
        }
    }

    // Collision detection / Move object
    // Remove calculation between object that has been calculate before
    // This will optimize the code a bit
    std::vector<std::vector<unsigned int>> pairs = collision::collisionPairing(objects);
    
    for (int i = 0; i < pairs.size(); i++)
    {
        std::vector<unsigned int> currentPair = pairs.at(i);

        std::shared_ptr<object::objectBaseClass> currentObject = objects.at(currentPair.at(0));
        currentObject->changeCollisionTime(deltaTime);
    }

    for (int i = 0; i < pairs.size(); i++)
    {
        std::vector<unsigned int> currentPair = pairs.at(i);

        std::cout << "Current object ID: " << currentPair.at(0) << std::endl;
        std::shared_ptr<object::objectBaseClass> currentObject = objects.at(currentPair.at(0));
        float objRemainingTime = currentObject->getCollisionTime();

        std::cout << "-  Before collision stat:" << std::endl;
        std::cout << "   -  Remaining time: " << objRemainingTime << std::endl;
        std::cout << "   -  Velocity: " << currentObject->getVelocity().x << ", " << currentObject->getVelocity().y << ", " << currentObject->getVelocity().z << std::endl;
        std::cout << "   -  Position: " << currentObject->getPosition().x << ", " << currentObject->getPosition().y << ", " << currentObject->getPosition().z << std::endl;

        while (objRemainingTime != 0.0f && currentPair.size() > 1)
        {
            float beforeTime = objRemainingTime;
            for (int j = 1; j < currentPair.size(); j++)
            {
                std::cout << "-  Target object ID: " << currentPair.at(j) << std::endl;
                std::shared_ptr<object::objectBaseClass> targetObject = objects.at(currentPair.at(j));
                float tarRemainingTime = targetObject->getCollisionTime();
                if (tarRemainingTime == 0.0f) continue;

                //float remainingTime = (objRemainingTime < tarRemainingTime) ? objRemainingTime : tarRemainingTime;
                std::vector<collision::collisionType> collisionResults = collision::CCD(currentObject, targetObject, deltaTime);

                unsigned int collideAxis = 0;
                std::vector<unsigned int> newlyCollideAxis;
                std::cout << "   -  Collision result: " << std::endl;
                for (unsigned int axis = 0; axis < 3; axis++)
                {
                    std::cout << "      -  Axis: " << axis << ", Collision type: ";
                    switch (collisionResults[axis])
                    {
                        case collision::collisionType::INSIDE:
                            collideAxis++;
                            std::cout << "Inside" << std::endl;
                            break;
                        case collision::collisionType::CROSS:
                            collideAxis++;
                            std::cout << "Cross" << std::endl;
                            break;
                        case collision::collisionType::NEWLY:
                            collideAxis++;
                            newlyCollideAxis.push_back(axis);
                            std::cout << "Newly" << std::endl;
                            break;
                        case collision::collisionType::NO:
                            std::cout << "No" << std::endl;
                            break;
                    }
                }

                if (collideAxis == 3 && newlyCollideAxis.size() > 0)
                {
                    std::cout << "   -  Collide" << std::endl;
                    float usageTime = collision::collisionResolver(currentObject, targetObject, deltaTime, newlyCollideAxis);
                    currentObject->changeCollisionTime(-usageTime);
                    targetObject->changeCollisionTime(-usageTime);

                    objRemainingTime = currentObject->getCollisionTime();
                }
                else
                {
                    std::cout << "   -  Not collide" << std::endl;
                }
            }
            // No collision
            if (beforeTime == objRemainingTime) break;
        }
        currentObject->move(fundamental::calculateDst(currentObject->getVelocity(), objRemainingTime));
        currentObject->changeCollisionTime(-objRemainingTime);

        std::cout << "-  End of collision stat:" << std::endl;
        std::cout << "   -  Remaining time: " << objRemainingTime << std::endl;
        std::cout << "   -  Velocity: " << currentObject->getVelocity().x << ", " << currentObject->getVelocity().y << ", " << currentObject->getVelocity().z << std::endl;
        std::cout << "   -  Position: " << currentObject->getPosition().x << ", " << currentObject->getPosition().y << ", " << currentObject->getPosition().z << std::endl;
    }
    std::cout << std::endl;
}