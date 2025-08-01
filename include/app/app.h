#pragma once

#include <cuda_runtime.h>

#include <graphics/vulkanCore.h>
#include <graphics/window.h>
#include <graphics/pipeline.h>
#include <graphics/renderer.h>
#include <graphics/vertex.h>

namespace auswahlen
{
    class app
    {
        private:
            /*------------------------------------------------------------
                App variables.
            ------------------------------------------------------------*/
            int width;
            int height;
            graphics::window guiWindow;
            graphics::pipeline pipeline;
            graphics::vulkanCore vulkan;
            graphics::renderer renderer;

            const std::vector<graphics::vertex> vertices = {
                {{ 0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                {{ 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
                {{-0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}}
            };

            /*------------------------------------------------------------
                Helper funcions.
            ------------------------------------------------------------*/
            // For constructor.
            void initGLFW();
            void initWindow();
            void initVulkan();
            void initGraphicPipeline();
            void initRenderer();

            // For run function.
            void mainLoop();
            void renderFrame();
            void stepSimulation();
        public:
            app(int width, int height);
            ~app();
            void run();
    };
}
