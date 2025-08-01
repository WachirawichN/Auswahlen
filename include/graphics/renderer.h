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
                VkBuffer vertexBuffer;
                VkDeviceMemory vertexBufferMemory;
                std::vector<VkCommandBuffer> commandBuffers;
                // Synchronize objects.
                std::vector<VkSemaphore> imageAvailableSemaphores;
                std::vector<VkSemaphore> renderFinishedSemaphores;
                std::vector<VkFence> inFlightFences;
                // Multiple frames in flight variables
                const int maxFramesInFlight = 2;
                uint32_t currentFrame = 0;

                // Test vertices.
                std::vector<vertex> vertices;
                /*------------------------------------------------------------
                    Helper funcions.
                ------------------------------------------------------------*/
                // Initializer functions.
                void initFrameBuffer();
                void initCommandPool();
                void initVertexBuffer();
                void initCommandBuffers();
                void initSyncObjects();

                // Memory functions.
                uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

                // Command functions.
                void recordCommand(VkCommandBuffer buffer, uint32_t imageIdx);
            public:
                renderer() {}
                renderer(const vulkanCore* vulkanCore, const pipeline* pipeline, const std::vector<vertex>& testVertices);

                renderer(const renderer&) = delete;
                // For copying some basic data before initialization only.
                renderer& operator=(const renderer& renderer);

                void render();

                void init();
                void cleanUp();
        };
    }
}