#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <graphic/vulkan.h>

#include <string>
#include <vector>
#include <optional>
#include <fstream>

// Debug / Error message
#include <stdexcept>
#include <iostream>

namespace auswahlen
{
    namespace graphic
    {
        class pipeline
        {
            private:
                /*------------------------------------------------------------
                    Pipeline variables.
                ------------------------------------------------------------*/
                std::optional<std::string> shaderCodePath[2];
                std::vector<char> vertexCode;
                std::vector<char> fragmentCode;

                VkRenderPass renderPass;
                VkPipelineLayout pipelineLayout;
                VkPipeline graphicsPipeline;

                /*------------------------------------------------------------
                    Dynamic states that will be used.
                ------------------------------------------------------------*/
                std::vector<VkDynamicState> dynamicStates = {
                    VK_DYNAMIC_STATE_VIEWPORT,
                    VK_DYNAMIC_STATE_SCISSOR
                };

                /*------------------------------------------------------------
                    Helper funcions.
                ------------------------------------------------------------*/
                // Initializer functions.
                void initRenderPass();
                void initGraphicsPipeline();

                // Shader functions.
                static std::vector<char> readFile(const std::string& path);
                VkShaderModule createShaderModule(std::vector<char> shaderCode, VkDevice device);
            public:
                pipeline() {}
                pipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

                pipeline(const pipeline&) = delete;
                pipeline& operator=(const pipeline& pipeline);

                void init(const vulkan& vulkan);
                void cleanUp(const vulkan& vulkan);
        };
    }
}