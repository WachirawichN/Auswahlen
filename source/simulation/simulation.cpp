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
        currentObject->changeCollisionTime(glm::vec3(deltaTime));
    }

    for (int i = 0; i < pairs.size(); i++)
    {
        std::vector<unsigned int> currentPair = pairs.at(i);

        std::shared_ptr<object::objectBaseClass> currentObject = objects.at(currentPair.at(0));
        glm::vec3 remainingTime = currentObject->getCollisionTime();

        while (mathExt::pythagoras(remainingTime) != 0.0f && currentPair.size() > 1)
        {
            glm::vec3 beforeTime = remainingTime;
            for (int j = 1; j < currentPair.size(); j++)
            {
                std::shared_ptr<object::objectBaseClass> targetObject = objects.at(currentPair.at(j));
                glm::vec3 timeToCollide = collision::dstBaseCD(currentObject, targetObject, remainingTime);

                if (mathExt::pythagoras(timeToCollide) != 0.0f)
                {
                    collision::collsionResolver(currentObject, targetObject, timeToCollide);
                    remainingTime -= timeToCollide;
                    targetObject->changeCollisionTime(timeToCollide * -1.0f);
                }
            }
            // No collision
            if (beforeTime == remainingTime) break;
        }
        currentObject->move(fundamental::calculateDstVecT(currentObject->getVelocity(), remainingTime));
        currentObject->changeCollisionTime(currentObject->getCollisionTime() * -1.0f);
    }
}