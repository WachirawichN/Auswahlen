#include <graphic/pipeline.h>

namespace auswahlen
{
    namespace graphic
    {
        /*------------------------------------------------------------
            Helper functions.
        ------------------------------------------------------------*/
        // initializer functions.
        void pipeline::initRenderPass()
        {

        }
        void pipeline::initGraphicsPipeline()
        {

        }
        
        // Shader functions.
        std::vector<char> pipeline::readFile(const std::string& path)
        {
            // Read at the end of the file.
            std::ifstream file(path, std::ios::ate | std::ios::binary);
            if (!file.is_open())
            {
                throw std::runtime_error("Unable to open file: " + path);
            }
    
            // The file is last position so tellg will just return the last position aka size.
            size_t fileSize = static_cast<size_t>(file.tellg());
            std::vector<char> buffer(fileSize);
    
            // Find first position, then read up from there.
            file.seekg(0);
            file.read(buffer.data(), fileSize);
            file.close();
    
            return buffer;
        }
        VkShaderModule pipeline::createShaderModule(std::vector<char> shaderCode, VkDevice device)
        {
            VkShaderModule shaderModule;
            VkShaderModuleCreateInfo createInfo = {
                .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                .codeSize = shaderCode.size(),
                .pCode = reinterpret_cast<const uint32_t*>(shaderCode.data())
            };

            if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
            {
                std::runtime_error("Failed to create shader module.");
            }
            return shaderModule;
        }

        /*------------------------------------------------------------
            Public functions.
        ------------------------------------------------------------*/
        pipeline::pipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
        {
            shaderCodePath[0] = vertexShaderPath;
            shaderCodePath[1] = fragmentShaderPath;
        }

        pipeline& pipeline::operator=(const pipeline& pipeline)
        {
            // Check if there are already vertex / fragment code assign to this variable yet, and check if it try to use "=" operator on itself.
            if (!(shaderCodePath[0].has_value() || shaderCodePath[1].has_value()) && (pipeline.shaderCodePath[0].has_value() && pipeline.shaderCodePath[1].has_value()) && this != &pipeline)
            {
                shaderCodePath[0] = pipeline.shaderCodePath[0];
                shaderCodePath[1] = pipeline.shaderCodePath[1];
            }
            return *this;
        }
    
        void pipeline::init(const vulkan& vulkan)
        {
            std::cout << "Initializing graphics pipeline." << std::endl;

            // Viewport and scissor properties.
            VkViewport viewport = {
                .x = 0.0f,
                .y = 0.0f,
                .width = (float)vulkan.getImgExtent().width,
                .height = (float)vulkan.getImgExtent().height,
                .minDepth = 0.0f,
                .maxDepth = 1.0f
            };
            VkRect2D scissor = {
                .offset = {0, 0},
                .extent = vulkan.getImgExtent()
            };

            // Turn raw SPIR-V code into shader module.
            vertexCode = readFile(shaderCodePath[0].value());
            fragmentCode = readFile(shaderCodePath[1].value());
            VkShaderModule vertModule = createShaderModule(vertexCode, vulkan.getDevice());
            VkShaderModule fragModule = createShaderModule(fragmentCode, vulkan.getDevice());

            // Create shader stages to actually use the shaders.
            VkPipelineShaderStageCreateInfo vertStageCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage = VK_SHADER_STAGE_VERTEX_BIT,
                .module = vertModule,
                .pName = "main"
            };
            VkPipelineShaderStageCreateInfo fragStageCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
                .module = fragModule,
                .pName = "main"
            };
            VkPipelineShaderStageCreateInfo shaderStageCreateInfo[2] = {vertStageCreateInfo, fragStageCreateInfo};

            // Essential create info.
            VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                .dynamicStateCount = (uint32_t)dynamicStates.size(),
                .pDynamicStates = dynamicStates.data()
            };
            VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
                .vertexBindingDescriptionCount = 0,
                .pVertexBindingDescriptions = nullptr,
                .vertexAttributeDescriptionCount = 0,
                .pVertexAttributeDescriptions = nullptr
            };
            VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                .primitiveRestartEnable = VK_FALSE
            };
            VkPipelineViewportStateCreateInfo viewportCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                .viewportCount = 1,
                .pViewports = &viewport,
                .scissorCount = 1,
                .pScissors = &scissor
            };
            VkPipelineRasterizationStateCreateInfo rasterizationCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                .depthClampEnable = VK_FALSE,
                .rasterizerDiscardEnable = VK_FALSE,
                .polygonMode = VK_POLYGON_MODE_FILL,
                .cullMode = VK_CULL_MODE_BACK_BIT,
                .frontFace = VK_FRONT_FACE_CLOCKWISE,
                .depthBiasEnable = VK_FALSE,
                .depthBiasConstantFactor = 0.0f,
                .depthBiasClamp = 0.0f,
                .depthBiasSlopeFactor = 0.0f,
                .lineWidth = 1.0f,
            };
            VkPipelineMultisampleStateCreateInfo multiSampleCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
                .sampleShadingEnable = VK_FALSE,
                .minSampleShading = 1.0f,
                .pSampleMask = nullptr,
                .alphaToCoverageEnable = VK_FALSE,
                .alphaToOneEnable = VK_FALSE
            };
            VkPipelineColorBlendAttachmentState colorBlendAttatchmentState = {
                .blendEnable = VK_FALSE,
                .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
                .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
                .colorBlendOp = VK_BLEND_OP_ADD,
                .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
                .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
                .alphaBlendOp = VK_BLEND_OP_ADD,
                .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
            };
            VkPipelineColorBlendStateCreateInfo colorBlendCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                .logicOpEnable = VK_FALSE,
                .logicOp = VK_LOGIC_OP_COPY,
                .attachmentCount = 1,
                .pAttachments = &colorBlendAttatchmentState,
                .blendConstants = {0.0f, 0.0f, 0.0f, 0.0f}
            };
            VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
                .setLayoutCount = 0,
                .pSetLayouts = nullptr,
                .pushConstantRangeCount = 0,
                .pPushConstantRanges = nullptr
            };

            // Create graphics pipeline layout.
            if (vkCreatePipelineLayout(vulkan.getDevice(), &pipelineLayoutCreateInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
            {
                std::runtime_error("Failed to created graphics pipeline layout.");
            }

            // Render pass and its essential struct.
            VkAttachmentDescription colorAttachment = {
                .format = vulkan.getFormat(),
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
            };
            VkAttachmentReference colorAttachmentRef = {
                .attachment = 0,
                .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
            };
            VkSubpassDescription subpassDescription = {
                .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                .colorAttachmentCount = 1,
                .pColorAttachments = &colorAttachmentRef
            };
            VkRenderPassCreateInfo renderPassCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
                .attachmentCount = 1,
                .pAttachments = &colorAttachment,
                .subpassCount = 1,
                .pSubpasses = &subpassDescription
            };
            
            // Create render pass,
            if (vkCreateRenderPass(vulkan.getDevice(), &renderPassCreateInfo, nullptr, &renderPass) != VK_SUCCESS)
            {
                std::runtime_error("Failed to create render pass.");
            }
            
            // Graphics pipeline's create info.
            VkGraphicsPipelineCreateInfo pipelineCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
                .stageCount = 2,
                .pStages = shaderStageCreateInfo,
                .pVertexInputState = &vertexInputCreateInfo,
                .pInputAssemblyState = &inputAssemblyCreateInfo,
                .pTessellationState = nullptr,
                .pViewportState = &viewportCreateInfo,
                .pRasterizationState = &rasterizationCreateInfo,
                .pMultisampleState = &multiSampleCreateInfo,
                .pDepthStencilState = nullptr,
                .pColorBlendState = &colorBlendCreateInfo,
                .pDynamicState = &dynamicStateCreateInfo,
                .layout = pipelineLayout,
                .renderPass = renderPass,
                .subpass = 0,
                .basePipelineHandle = VK_NULL_HANDLE,
                .basePipelineIndex = -1
            };

            // Creating the graphics pipeline itself.
            if (vkCreateGraphicsPipelines(vulkan.getDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
            {
                std::runtime_error("Failed to create graphics pipeline");
            }

            // Shader module will not be used now.
            vkDestroyShaderModule(vulkan.getDevice(), vertModule, nullptr);
            vkDestroyShaderModule(vulkan.getDevice(), fragModule, nullptr);
        }
        void pipeline::cleanUp(const vulkan& vulkan)
        {
            std::cout << "Cleaning up graphics pipeline." << std::endl;

            vkDestroyPipeline(vulkan.getDevice(), graphicsPipeline, nullptr);
            vkDestroyRenderPass(vulkan.getDevice(), renderPass, nullptr);
            vkDestroyPipelineLayout(vulkan.getDevice(), pipelineLayout, nullptr);
        }
    }
}