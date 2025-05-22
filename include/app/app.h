#pragma once

#include <graphics/vulkanCore.h>
#include <graphics/window.h>
#include <graphics/pipeline.h>
#include <graphics/renderer.h>

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
