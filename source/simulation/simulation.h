#pragma once

#include "object/objectBaseClass.h"

#include "../graphic/graphic.h"
#include "../graphic/geometry/shape.h"

#include <vector>
#include <memory>

// Handle all the rendering and the physic in the scene
class simulation
{
    private:
        std::vector<std::shared_ptr<object::objectBaseClass>> objects;

        float gravity;

        renderer workspaceRenderer;
        camera* mainCamera;
        shader programShader;
    public:
        simulation(camera* mainCamera, shader programShader, float gravity = -9.8f);

        void addObject(std::shared_ptr<object::objectBaseClass> newObject);
        void drawSimulation();
        void updateSimulation(float deltaTime);
};
