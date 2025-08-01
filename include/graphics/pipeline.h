/*

    A graphic pipeline class use to configure and create the stages of the pipeline and render pass.

*/

#pragma once

// Vulkan
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <graphics/vulkanCore.h>
#include <graphics/vertex.h>

// std
#include <string>
#include <vector>
#include <array>
#include <optional>
#include <fstream>

// Debug / Error message
#include <stdexcept>
#include <iostream>

namespace auswahlen
{
    namespace graphics
    {
        class pipeline
        {
            private:
                /*------------------------------------------------------------
                    Graphics pipeline variables.
                ------------------------------------------------------------*/
                const vulkanCore* vulkan;

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
                    Render pass / Graphics pipeline config info.
                ------------------------------------------------------------*/
                struct renderPassConfigInfo {
                    VkAttachmentDescription colorAttachment;
                    VkAttachmentReference colorAttachmentRef;
                    VkSubpassDescription subpassDescription;
                    VkSubpassDependency dependency;
                    VkRenderPassCreateInfo renderPassCreateInfo;
                    uint32_t subpassCount = 1;
                };
                struct pipelineConfigInfo {
                    VkViewport viewport;
                    VkRect2D scissor;
                    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
                    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
                    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
                    VkPipelineViewportStateCreateInfo viewportInfo;
                    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
                    VkPipelineMultisampleStateCreateInfo multisampleInfo;
                    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
                    VkPipelineColorBlendAttachmentState colorBlendAttachmentInfo;
                    VkPipelineColorBlendStateCreateInfo colorBlenStateInfo;
                    VkPipelineLayoutCreateInfo pipelineLayout;
                };
                

                /*------------------------------------------------------------
                    Helper funcions.
                ------------------------------------------------------------*/
                // Initializer functions.
                void initRenderPass();
                void initGraphicsPipeline();

                // Shader functions.
                static std::vector<char> readFile(const std::string& path);
                VkShaderModule createShaderModule(std::vector<char> shaderCode);

                // Render pass / Graphics pipeline config functions.
                const renderPassConfigInfo populateRenderPassConfig();
                const pipelineConfigInfo populatePipelineConfig(const VkVertexInputBindingDescription& vertexBindingDescription, const std::array<VkVertexInputAttributeDescription, 2>& vertexAttributeDescriptions);
            public:
                pipeline() {}
                pipeline(const vulkanCore* vulkanCore, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

                pipeline(const pipeline&) = delete;
                // For copying some basic data before initialization only.
                pipeline& operator=(const pipeline& pipeline);

                const VkRenderPass& getRenderPass() const { return renderPass; }
                const VkPipeline& getPipeline() const { return graphicsPipeline; }

                void init();
                void cleanUp();
        };
    }
}