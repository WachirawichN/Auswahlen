#pragma once

#include <cuda_runtime.h>

#include <GLC/vector.cuh>

#include <graphics/vertex.h>

namespace auswahlen
{
    class simulation
    {
        private:
            const std::vector<graphics::vertex> vertices = {
                {{ 0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                {{ 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
                {{-0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}}
            };
        public:
            simulation();
            ~simulation();
            void stepSimulation();
    };
}
