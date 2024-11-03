#include "simulation.h"

simulation::simulation(camera* mainCamera, shader programShader)
    : mainCamera(mainCamera), programShader(programShader)
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

    // Render objects
    for (size_t i = 0; i < objects.size(); i++)
    {
        std::shared_ptr<object::objectBaseClass> currentObject = objects.at(i);

        geometry::shape* shapePart = dynamic_cast<geometry::shape*>(currentObject.get());
        if (shapePart) // Check if object is inheritance of shape class
        {
            std::vector<float> vertices = shapePart->getVertices();
            std::vector<unsigned int> indices = shapePart->getIndices();

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
            // Make the rotation be within 360 degree
            for (int i = 0; i < 3; i++)
            {
                if (objectRotation[i] > 360 || objectRotation[i] < 0)
                {
                    int multiplier = floor(objectRotation[i] / 360.0f);
                    objectRotation[i] -= multiplier * 360.0f;
                }
            }
            float degree = std::max(objectRotation.x, std::max(objectRotation.y, objectRotation.z));
            if (degree != 0)
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