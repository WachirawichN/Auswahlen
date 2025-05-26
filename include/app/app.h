#pragma once

#include <cuda_runtime.h>
#include <GLC/vector.cuh>
#include <GLC/matrix.cuh>
#include <GLC/utility.cuh>

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
