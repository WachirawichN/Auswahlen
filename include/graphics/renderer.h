/*

    A class that creates and contains variables that are necessary for drawing image to the image views inside the vulkanCore class.

*/

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <graphics/vulkanCore.h>
#include <graphics/pipeline.h>

#include <vector>
#include <cstdint>

// Debug / Error message
#include <stdexcept>
#include <iostream>

namespace auswahlen
{
    namespace graphics
    {
        class renderer
        {
            private:
                /*------------------------------------------------------------
                    Renderer variables.
                ------------------------------------------------------------*/
                // Essential variables for initialization.
                const vulkanCore* vulkan;
                const pipeline* graphicsPipeline;
                
                // Variables that are actually the one that responsible for rendering.
                std::vector<VkFramebuffer> frameBuffers;
                VkCommandPool commandPool;
                VkCommandBuffer commandBuffer;
                VkSemaphore imageAvailableSemaphore;
                VkSemaphore renderFinishedSemaphore;
                VkFence inFlightFence;

                const int maxFramesInFlight = 2;

                /*------------------------------------------------------------
                    Helper funcions.
                ------------------------------------------------------------*/
                // Initializer functions.
                void initFrameBuffer();
                void initCommandBuffer();
                void initSyncObjects();

                // Command functions.
                void recordCommand(VkCommandBuffer buffer, uint32_t imageIdx);
            public:
                renderer() {}
                renderer(const vulkanCore* vulkanCore, const pipeline* pipeline);

                renderer(const renderer&) = delete;
                // For copying some basic data before initialization only.
                renderer& operator=(const renderer& renderer);

                void render();

                void init();
                void cleanUp();
        };
    }
}