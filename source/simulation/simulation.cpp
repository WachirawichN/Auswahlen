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
            //std::cout << "Rotation: " << objectRotation.x << ", " << objectRotation.y << ", " << objectRotation.z << std::endl;
            float degree = std::max(objectRotation.x, std::max(objectRotation.y, objectRotation.z));
            if (degree != 0.0f)
            {
                modelMatrix = glm::rotate(modelMatrix, glm::radians(degree), glm::vec3(objectRotation.x / degree, objectRotation.y / degree, objectRotation.z / degree));
                //std::cout << "Degree: " << degree << " , " << objectRotation.x / degree << ", " << objectRotation.y / degree << ", " << objectRotation.z / degree << std::endl;
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

    // Remove calculation between object that has been calculate before
    std::vector<std::vector<unsigned int>> pairs = collision::collisionPairing(objects);
    /*
    for (int i = 0; i < pairs.size(); i++)
    {
        std::vector<unsigned int> currentPair = pairs.at(i);
        for (int j = 0; j < currentPair.size(); j++)
        {
            std::cout << currentPair.at(j) << ", ";
        }
        std::cout << std::endl;
    }
    */
    
    for (int i = 0; i < pairs.size(); i++)
    {
        std::vector<unsigned int> currentPair = pairs.at(i);

        std::shared_ptr<object::objectBaseClass> currentObject = objects.at(currentPair.at(0));
        std::cout << "Object ID: " << currentPair.at(0) << std::endl;
        glm::vec3 remainingTime(deltaTime);

        while (mathExt::pythagoras(remainingTime) != 0.0f && currentPair.size() > 1)
        {
            glm::vec3 beforeTime = remainingTime;
            for (int j = 1; j < currentPair.size(); j++)
            {
                std::shared_ptr<object::objectBaseClass> targetObject = objects.at(currentPair.at(j));
                std::cout << "  -  Target ID: " << currentPair.at(j) << std::endl;
                glm::vec3 timeToCollide = collision::dstBaseCD(currentObject, targetObject, deltaTime);

                if (mathExt::pythagoras(timeToCollide) != 0.0f)
                {
                    collision::collsionResolver(currentObject, targetObject, timeToCollide);
                    remainingTime -= timeToCollide;
                }
                std::cout << "      -  After resolving" << std::endl;
                std::cout << "         -  Object velocity: " << currentObject->getVelocity().x << ", " << currentObject->getVelocity().y << ", " << currentObject->getVelocity().z << std::endl;
                std::cout << "         -  Target velocity: " << targetObject->getVelocity().x << ", " << targetObject->getVelocity().y << ", " << targetObject->getVelocity().z << std::endl;
            }
            if (beforeTime == remainingTime) break;
        }
        std::cout << currentObject->getPosition().x << ", " << currentObject->getPosition().y << ", " << currentObject->getPosition().z << std::endl;
        currentObject->move(fundamental::calculateDstVecT(currentObject->getVelocity(), remainingTime));
        std::cout << currentObject->getPosition().x << ", " << currentObject->getPosition().y << ", " << currentObject->getPosition().z << std::endl;
    }

    /*
    for (int i = 0; i < objects.size(); i++)
    {
        std::shared_ptr<object::objectBaseClass> currentObject = objects.at(i);
        
        std::cout << "Object ID: " << i << std::endl;

        // Collision detection
        glm::vec3 remainingTime(deltaTime);
        if (currentObject->canCollide() && !(currentObject->isAnchored()) && objects.size() > 1)
        {
            while (mathExt::pythagoras(remainingTime) != 0.0f)
            {
                glm::vec3 beforeTime = remainingTime;

                // Loop through every target objects
                for (int j = 0; j < objects.size(); j++)
                {
                    if (j == i) continue;

                    std::shared_ptr<object::objectBaseClass> targetObject = objects.at(j);
                    if (!targetObject->canCollide()) continue;

                    std::cout << "  -  Target ID: " << j << std::endl;
                    glm::vec3 timeToCollide = collision::dstBaseCD(currentObject, targetObject, deltaTime);

                    std::cout << mathExt::pythagoras(timeToCollide) << std::endl;
                    if (mathExt::pythagoras(timeToCollide) != 0.0f)
                    {
                        collision::collsionResolver(currentObject, targetObject, timeToCollide);
                        remainingTime -= timeToCollide;
                        std::cout << "Resolve" << std::endl;
                    }
                }

                // There is no collision with any of the target
                if (beforeTime == remainingTime) break;
            }
        }
        currentObject->move(fundamental::calculateDstVecT(currentObject->getVelocity(), remainingTime));
    }
    */
    std::cout << std::endl;
}