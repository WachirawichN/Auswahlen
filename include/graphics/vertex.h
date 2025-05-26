#pragma once

// CUDA
#include <cuda_runtime.h>

// Vulkan
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <graphics/vulkanCore.h>

// GLC
#include <GLC/vector.cuh>

// std
#include <array>

namespace auswahlen
{
    namespace graphics
    {
        struct vertex
        {
            GLC::vec2 position;
            GLC::vec3 color;

            static VkVertexInputBindingDescription bindingDescription()
            {
                VkVertexInputBindingDescription description = {
                    .binding = 0,
                    .stride = sizeof(vertex),
                    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
                };
                return description;
            };
            static std::array<VkVertexInputAttributeDescription, 2> attributeDescription()
            {
                std::array<VkVertexInputAttributeDescription, 2> descriptions = {
                    VkVertexInputAttributeDescription{
                        .location = 0,
                        .binding = 0,
                        .format = VK_FORMAT_R32G32_SFLOAT,
                        .offset = offsetof(vertex, position)
                    },
                };
                return descriptions;
            }
        };
    }
}