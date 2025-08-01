#pragma once

// CUDA
#include <cuda_runtime.h>

// Vulkan
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <graphics/vulkanCore.h>

// GLC
#include <GLC/GLC.cuh>

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
                VkVertexInputBindingDescription desc{};
                desc.binding = 0;
                desc.stride = sizeof(vertex);
                desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

                return desc;
                /*
                VkVertexInputBindingDescription bindingDescription = {
                    .binding = 0,
                    .stride = sizeof(vertex),
                    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
                };
                return bindingDescription;
                */
            };
            static std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions()
            {
                std::array<VkVertexInputAttributeDescription, 2> descs{};
                descs[0].location = 0;
                descs[0].binding = 0;
                descs[0].format = VK_FORMAT_R32G32_SFLOAT;
                descs[0].offset = offsetof(vertex, position);

                descs[1].location = 1;
                descs[1].binding = 0;
                descs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
                descs[1].offset = offsetof(vertex, color);

                return descs;

                /*
                std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {
                    VkVertexInputAttributeDescription {
                        .location = 0,
                        .binding = 0,
                        .format = VK_FORMAT_R32G32_SFLOAT,
                        .offset = offsetof(vertex, position)
                    },
                    VkVertexInputAttributeDescription {
                        .location = 1,
                        .binding = 0,
                        .format = VK_FORMAT_R32G32B32_SFLOAT,
                        .offset = offsetof(vertex, color)
                    }
                };
                return attributeDescriptions;
                */
            }
        };
    }
}