#pragma once

#include <graphic/vulkan.h>
#include <graphic/window.h>
#include <graphic/pipeline.h>

namespace auswahlen
{
    class app
    {
        private:
            // graphic
            int width;
            int height;
            graphic::window guiWindow;
            pipeline pipeline{"asset/shader/shader.vert.spv", "asset/shader/shader.frag.spv"};

            graphic::vulkan vulkan;

            // Helper functions.
            // For constructor.
            void initWindow();
            void initVulkan();

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
