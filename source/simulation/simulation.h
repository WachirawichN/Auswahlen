#pragma once

#include "object/objectBaseClass.h"
#include "object/sphere.h"
#include "object/cube.h"

#include "physic/projectileMotion.h"
#include "physic/collision.h"

#include "../graphic/graphic.h"
#include "../graphic/geometry/shape.h"

#include <vector>
#include <memory>
#include <typeinfo>

// Handle all the rendering and the physic in the scene
class simulation
{
    private:
        std::vector<std::shared_ptr<object::objectBaseClass>> objects;

        float gravity;

        renderer workspaceRenderer;
        camera* mainCamera;
        shader programShader;

        unsigned int gpuCoreCount;
    public:
        simulation(camera* mainCamera, shader programShader, float gravity = -9.8f);

        void addObject(std::shared_ptr<object::objectBaseClass> newObject);
        void drawSimulation();
        void updateSimulation(float deltaTime);
};
