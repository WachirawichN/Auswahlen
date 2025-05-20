#include <graphics/pipeline.h>

namespace auswahlen
{
    namespace graphics
    {
        /*------------------------------------------------------------
            Helper functions.
        ------------------------------------------------------------*/
        // initializer functions.
        void pipeline::initRenderPass()
        {
            std::cout << "\t- Initializing render pass." << std::endl;

            // Render pass and its essentials config info.
            renderPassConfigInfo renderPassConfig = populateRenderPassConfig();
            
            // Create render pass,
            if (vkCreateRenderPass(vulkan->getDevice(), &renderPassConfig.renderPassCreateInfo, nullptr, &renderPass) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create render pass.");
            }
            std::cout << "\t\t- Initialization completed." << std::endl;
        }
        void pipeline::initGraphicsPipeline()
        {
            std::cout << "\t- Initializing graphics pipeline." << std::endl;

            // Turn raw SPIR-V code into shader module.
            vertexCode = readFile(shaderCodePath[0].value());
            fragmentCode = readFile(shaderCodePath[1].value());
            VkShaderModule vertModule = createShaderModule(vertexCode);
            VkShaderModule fragModule = createShaderModule(fragmentCode);

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

            // Viewport / Scissor / Graphics pipeline stages config info.
            pipelineConfigInfo pipelineConfig = populatePipelineConfig();

            // Create graphics pipeline layout.
            if (vkCreatePipelineLayout(vulkan->getDevice(), &pipelineConfig.pipelineLayout, nullptr, &pipelineLayout) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to created graphics pipeline layout.");
            }
            
            // Graphics pipeline's create info.
            VkGraphicsPipelineCreateInfo pipelineCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
                .stageCount = 2,
                .pStages = shaderStageCreateInfo,
                .pVertexInputState = &pipelineConfig.vertexInputInfo,
                .pInputAssemblyState = &pipelineConfig.inputAssemblyInfo,
                .pTessellationState = nullptr,
                .pViewportState = &pipelineConfig.viewportInfo,
                .pRasterizationState = &pipelineConfig.rasterizationInfo,
                .pMultisampleState = &pipelineConfig.multisampleInfo,
                .pDepthStencilState = &pipelineConfig.depthStencilInfo,
                .pColorBlendState = &pipelineConfig.colorBlenStateInfo,
                .pDynamicState = &pipelineConfig.dymicStateInfo,
                .layout = pipelineLayout,
                .renderPass = renderPass,
                .subpass = 0,
                .basePipelineHandle = VK_NULL_HANDLE,
                .basePipelineIndex = -1
            };

            // Creating the graphics pipeline itself.
            if (vkCreateGraphicsPipelines(vulkan->getDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create graphics pipeline.");
            }
            
            // Shader module will not be used after graphics pipeline creation.
            vkDestroyShaderModule(vulkan->getDevice(), vertModule, nullptr);
            vkDestroyShaderModule(vulkan->getDevice(), fragModule, nullptr);

            std::cout << "\t\t- Initialization completed." << std::endl;
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
        VkShaderModule pipeline::createShaderModule(std::vector<char> shaderCode)
        {
            VkShaderModule shaderModule;
            VkShaderModuleCreateInfo createInfo = {
                .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                .codeSize = shaderCode.size(),
                .pCode = reinterpret_cast<const uint32_t*>(shaderCode.data())
            };

            if (vkCreateShaderModule(vulkan->getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create shader module.");
            }
            return shaderModule;
        }

        // Render pass / Graphics pipeline config functions.
        const pipeline::renderPassConfigInfo pipeline::populateRenderPassConfig()
        {
            // Populate all create infos that are essential for render pass creation.
            pipeline::renderPassConfigInfo config;
            config.colorAttachment = {
                .format = vulkan->getFormat(),
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
            };
            config.colorAttachmentRef = {
                .attachment = 0,
                .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
            };
            config.subpassDescription = {
                .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                .colorAttachmentCount = 1,
                .pColorAttachments = &config.colorAttachmentRef
            };
            config.renderPassCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
                .attachmentCount = 1,
                .pAttachments = &config.colorAttachment,
                .subpassCount = config.subpassCount,
                .pSubpasses = &config.subpassDescription
            };

            return config;
        }
        const pipeline::pipelineConfigInfo pipeline::populatePipelineConfig()
        {
            // Populate all create infos that are essential for graphics pipeline creation.
            // Both the viewport and scissor pair and all the stage of the pipeline.
            pipeline::pipelineConfigInfo config;

            config.viewport = {
                .x = 0.0f,
                .y = 0.0f,
                .width = static_cast<float>(vulkan->getImgExtent().width),
                .height = static_cast<float>(vulkan->getImgExtent().height),
                .minDepth = 0.0f,
                .maxDepth = 1.0f
            };
            config.scissor = {
                .offset = {0, 0},
                .extent = vulkan->getImgExtent()
            };

            config.dymicStateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
                .pDynamicStates = dynamicStates.data()
            };
            config.vertexInputInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
                .vertexBindingDescriptionCount = 0,
                .pVertexBindingDescriptions = nullptr,
                .vertexAttributeDescriptionCount = 0,
                .pVertexAttributeDescriptions = nullptr
            };
            config.inputAssemblyInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                .primitiveRestartEnable = VK_FALSE
            };
            config.viewportInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                .viewportCount = 1,
                .pViewports = &config.viewport,
                .scissorCount = 1,
                .pScissors = &config.scissor
            };
            config.rasterizationInfo = {
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
            config.multisampleInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
                .sampleShadingEnable = VK_FALSE,
                .minSampleShading = 1.0f,
                .pSampleMask = nullptr,
                .alphaToCoverageEnable = VK_FALSE,
                .alphaToOneEnable = VK_FALSE
            };
            config.depthStencilInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
                .depthTestEnable = VK_TRUE,
                .depthWriteEnable = VK_TRUE,
                .depthCompareOp = VK_COMPARE_OP_LESS,
                .depthBoundsTestEnable = VK_FALSE,
                .stencilTestEnable = VK_FALSE,
                .front = {},
                .back = {},
                .minDepthBounds = 0.0f,
                .maxDepthBounds = 1.0f
            };
            config.colorBlendAttachmentInfo = {
                .blendEnable = VK_FALSE,
                .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
                .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
                .colorBlendOp = VK_BLEND_OP_ADD,
                .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
                .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
                .alphaBlendOp = VK_BLEND_OP_ADD,
                .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
            };
            config.colorBlenStateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                .logicOpEnable = VK_FALSE,
                .logicOp = VK_LOGIC_OP_COPY,
                .attachmentCount = 1,
                .pAttachments = &config.colorBlendAttachmentInfo,
                .blendConstants = {0.0f, 0.0f, 0.0f, 0.0f}
            };
            config.pipelineLayout = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
                .setLayoutCount = 0,
                .pSetLayouts = nullptr,
                .pushConstantRangeCount = 0,
                .pPushConstantRanges = nullptr
            };
            return config;
        }

        /*------------------------------------------------------------
            Public functions.
        ------------------------------------------------------------*/
        pipeline::pipeline(const vulkanCore* vulkanCore, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
        {
            vulkan = vulkanCore;
            shaderCodePath[0] = vertexShaderPath;
            shaderCodePath[1] = fragmentShaderPath;
        }

        pipeline& pipeline::operator=(const pipeline& pipeline)
        {
            // Check if there are already vertex / fragment code assign to this variable yet, and check if it try to use "=" operator on itself.
            if (!(shaderCodePath[0].has_value() || shaderCodePath[1].has_value()) && (pipeline.shaderCodePath[0].has_value() && pipeline.shaderCodePath[1].has_value()) && this != &pipeline)
            {
                vulkan = pipeline.vulkan;
                shaderCodePath[0] = pipeline.shaderCodePath[0];
                shaderCodePath[1] = pipeline.shaderCodePath[1];
            }
            return *this;
        }
    
        void pipeline::init()
        {
            std::cout << "Initializing graphics pipeline." << std::endl;
            initRenderPass();
            initGraphicsPipeline();
        }
        void pipeline::cleanUp()
        {
            std::cout << "Cleaning up graphics pipeline." << std::endl;

            std::cout << "\t- Destroying graphics pipeline." << std::endl;
            vkDestroyPipeline(vulkan->getDevice(), graphicsPipeline, nullptr);

            std::cout << "\t- Destroying pipeline layout." << std::endl;
            vkDestroyPipelineLayout(vulkan->getDevice(), pipelineLayout, nullptr);
            
            std::cout << "\t- Destroying render pass." << std::endl;
            vkDestroyRenderPass(vulkan->getDevice(), renderPass, nullptr);
        }
    }
}