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