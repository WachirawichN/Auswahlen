#pragma once

#include <graphic/vulkan.h>
#include <graphic/window.h>
#include <graphic/pipeline.h>

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
            graphic::window guiWindow;
            graphic::pipeline pipeline;
            graphic::vulkan vulkan;

            /*------------------------------------------------------------
                Helper funcions.
            ------------------------------------------------------------*/
            // For constructor.
            void initGLFW();
            void initWindow();
            void initVulkan();
            void initGraphicPipeline();

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
