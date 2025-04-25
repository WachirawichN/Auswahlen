#pragma once

#include "object/objectBaseClass.cuh"
#include "object/sphere.h"
#include "object/cube.h"

#include "physic/fundamental.cuh"
#include "physic/collision.cuh"

#include "math/mathExt.cuh"

#include "../graphic/graphic.h"
#include "../graphic/geometry/shape.h"

#include <vector>
#include <memory>
#include <typeinfo>
#include <iomanip>
#include <algorithm>

// Handle all the rendering and the physic in the scene
class simulation
{
    private:
        std::shared_ptr<object::objectBaseClass> simulationBox;
        float gridSize;

        std::vector<std::shared_ptr<object::objectBaseClass>> objects;

        float gravity;

        renderer workspaceRenderer;
        camera* mainCamera;
        shader programShader;
    public:
        simulation(std::shared_ptr<object::objectBaseClass> simulationBox,
                   float gridSize,
                   camera* mainCamera,
                   shader programShader,
                   float gravity = -9.8f);

        void addObject(std::shared_ptr<object::objectBaseClass> newObject);
        void drawSimulation();
        void updateSimulation(float deltaTime);
};
