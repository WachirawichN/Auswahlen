#include <graphics/renderer.h>

namespace auswahlen
{
    namespace graphics
    {
        /*------------------------------------------------------------
            Helper functions.
        ------------------------------------------------------------*/
        // Initializer functions.
        void renderer::initFrameBuffer()
        {
            std::cout << "\t- Initializing frame buffers." << std::endl;
            frameBuffers.resize(vulkan->getImageViews().size());

            // Create frame buffer for each image view.
            for (size_t i = 0; i < vulkan->getImageViews().size(); i++)
            {
                VkImageView attatchments[] = {vulkan->getImageViews()[i]};

                VkFramebufferCreateInfo frameBufferCreateInfo = {
                    .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                    .renderPass = graphicsPipeline->getRenderPass(),
                    .attachmentCount = 1,
                    .pAttachments = attatchments,
                    .width = vulkan->getImageExtent().width,
                    .height = vulkan->getImageExtent().height,
                    .layers = 1
                };

                if (vkCreateFramebuffer(vulkan->getDevice(), &frameBufferCreateInfo, nullptr, &frameBuffers[i]) != VK_SUCCESS)
                {
                    throw std::runtime_error("Failed to create frame buffer(s).");
                }
            }
            std::cout << "\t\t- Initialization completed." << std::endl;
        }
        void renderer::initCommandPool()
        {
            std::cout << "\t- Initializing command Pool." << std::endl;

            // Command pool's create info, using graphic family queue (for drawing).
            VkCommandPoolCreateInfo commandPoolCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
                .queueFamilyIndex = vulkan->getQueueFamilyIndices().graphicsFamilyIdx.value()
            };

            // Create the drawing command pool.
            if (vkCreateCommandPool(vulkan->getDevice(), &commandPoolCreateInfo, nullptr, &commandPool) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create command pool.");
            }
            std::cout << "\t\t- Initialization completed." << std::endl;
        }
        void renderer::initVertexBuffer()
        {
            std::cout << "\t- Initializing vertex buffer." << std::endl;

            // Vertex buffer part
            VkBufferCreateInfo vertexBufferCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                .size = vertices.size() * sizeof(vertices[0]),
                .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            };
            if (vkCreateBuffer(vulkan->getDevice(), &vertexBufferCreateInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create vertex buffer.");
            }
            std::cout << "\t\t- Created vertex buffer." << std::endl;
            
            // Memory allocation.
            // Get vertex buffer requirement for memory allocation.
            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(vulkan->getDevice(), vertexBuffer, &memRequirements);

            // Allocate the memory.
            VkMemoryAllocateInfo memAllocationInfo = {
                .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                .allocationSize = memRequirements.size,
                .memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
            };
            if(vkAllocateMemory(vulkan->getDevice(), &memAllocationInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to allocate vertex buffer memory.");
            }
            std::cout << "\t\t- Allocated memory for vertex buffer." << std::endl;
            
            // Copy data.
            void* data;
            vkMapMemory(vulkan->getDevice(), vertexBufferMemory, 0, vertexBufferCreateInfo.size, 0, &data);
            memcpy(data, vertices.data(), static_cast<size_t>(vertexBufferCreateInfo.size));
            vkUnmapMemory(vulkan->getDevice(), vertexBufferMemory);
            std::cout << "\t\t- Copied vertex buffer data." << std::endl;
        }
        void renderer::initCommandBuffers()
        {
            std::cout << "\t- Initializing command buffer." << std::endl;
            commandBuffers.resize(maxFramesInFlight);
            VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                .commandPool = commandPool,
                .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                .commandBufferCount = static_cast<uint32_t>(commandBuffers.size())
            };
            if (vkAllocateCommandBuffers(vulkan->getDevice(), &commandBufferAllocateInfo, commandBuffers.data()) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to allocate command buffer.");
            }
            std::cout << "\t\t- Initialization completed." << std::endl;
        }
        void renderer::initSyncObjects()
        {
            std::cout << "\t- Initializing synchronization objects." << std::endl;

            imageAvailableSemaphores.resize(maxFramesInFlight);
            renderFinishedSemaphores.resize(maxFramesInFlight);
            inFlightFences.resize(maxFramesInFlight);

            VkSemaphoreCreateInfo semaphoreCreateInfo = {.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
            VkFenceCreateInfo fenceCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                .flags = VK_FENCE_CREATE_SIGNALED_BIT
            };

            for (size_t i = 0; i < maxFramesInFlight; i++)
            {
                if (vkCreateSemaphore(vulkan->getDevice(), &semaphoreCreateInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
                    vkCreateSemaphore(vulkan->getDevice(), &semaphoreCreateInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
                    vkCreateFence(vulkan->getDevice(), &fenceCreateInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
                {
                    throw std::runtime_error("Failed to create semaphore(s) or fence.");
                }
            }
            std::cout << "\t\t- Initialization completed." << std::endl;
        }

        // Memory functions.
        uint32_t renderer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
        {
            // Find the index of memory type that that have all of the required properties.

            // Get physical device's supported memory.
            VkPhysicalDeviceMemoryProperties memProperties;
            vkGetPhysicalDeviceMemoryProperties(vulkan->getPhysicalDevice(), &memProperties);

            for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
            {
                if ((typeFilter & (i << 1)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                {
                    return i;
                }
            }
            throw std::runtime_error("Cannot find the index of the suitable memory type.");
        }

        // Command functions.
        void renderer::recordCommand(VkCommandBuffer buffer, uint32_t imageIdx)
        {
            // Begin info that describe usage of the command buffer.
            VkCommandBufferBeginInfo commandBufferBeginInfo = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                .flags = 0
            };

            if (vkBeginCommandBuffer(buffer, &commandBufferBeginInfo) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to start recording command.");
            }

            // For use with the VK_ATTACHMENT_LOAD_OP_CLEAR config of the render pass.
            VkClearValue clearValue = {
                .color = VkClearColorValue({{0.0f, 0.0f, 0.0f, 1.0f}})
            };

            VkRect2D renderArea = {
                .offset = {0, 0},
                .extent = vulkan->getImageExtent()
            };
            VkRenderPassBeginInfo renderPassBeginInfo = {
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .renderPass = graphicsPipeline->getRenderPass(),
                .framebuffer = frameBuffers[imageIdx],
                .renderArea = renderArea,
                .clearValueCount = 1,
                .pClearValues = &clearValue
            };
            vkCmdBeginRenderPass(buffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            // Recording the command part.
            vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->getPipeline());

            // Set the viewport and scissor due to it is setted up to be dynamic.
            VkViewport viewport = {
                .x = 0.0f,
                .y = 0.0f,
                .width = static_cast<float>(vulkan->getImageExtent().width),
                .height = static_cast<float>(vulkan->getImageExtent().height),
                .minDepth = 0.0f,
                .maxDepth = 1.0f
            };
            vkCmdSetViewport(buffer, 0, 1, &viewport);
            VkRect2D scissor = {
                .offset = {0, 0},
                .extent = vulkan->getImageExtent()
            };
            vkCmdSetScissor(buffer, 0, 1, &scissor);

            // Bind multiple vertex buffers.
            VkBuffer vertexBuffers[] = {vertexBuffer};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(buffer, 0, 1, vertexBuffers, offsets);

            // Draw 3 vertices with 1 instance.
            vkCmdDraw(buffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);

            // End the command recording.
            vkCmdEndRenderPass(buffer);
            if (vkEndCommandBuffer(buffer) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to end recording command.");
            }
        }
        /*------------------------------------------------------------
            Public functions.
        ------------------------------------------------------------*/
        renderer::renderer(const vulkanCore* vulkanCore, const pipeline* pipeline, const std::vector<vertex>& testVertices)
            : vulkan(vulkanCore), graphicsPipeline(pipeline), vertices(testVertices)
        {
        }

        renderer& renderer::operator=(const renderer& renderer)
        {
            if (this != &renderer)
            {
                vulkan = renderer.vulkan;
                graphicsPipeline = renderer.graphicsPipeline;
                vertices = renderer.vertices;
            }
            return *this;
        }

        void renderer::render()
        {
            // Wait for the previous frame to finish rendering.
            vkWaitForFences(vulkan->getDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

            // Reset the fence to unsignaled state which mean the frame is currently being render.
            vkResetFences(vulkan->getDevice(), 1, &inFlightFences[currentFrame]);

            // Get the image index for choosing frame buffer.
            uint32_t imageIdx;
            vkAcquireNextImageKHR(vulkan->getDevice(), vulkan->getSwapChain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], nullptr, &imageIdx);
            
            // Recording the command buffer.
            vkResetCommandBuffer(commandBuffers[currentFrame], 0);
            recordCommand(commandBuffers[currentFrame], imageIdx);

            // Wait on which semaphores at which stage before execute the command.
            // Currently waiting for acquiring the frame buffer index only, before being able to drawn into that frame buffer.
            VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
            VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

            // Which semaphores will be signaled after finished rendering.
            VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};

            // Submit to graphics queue.
            VkSubmitInfo submitInfo = {
                .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                .waitSemaphoreCount = 1,
                .pWaitSemaphores = waitSemaphores,
                .pWaitDstStageMask = waitStages,
                .commandBufferCount = 1,
                .pCommandBuffers = &commandBuffers[currentFrame],
                .signalSemaphoreCount = 1,
                .pSignalSemaphores = signalSemaphores
            };
            if (vkQueueSubmit(vulkan->getGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to submitted command to queue.");
            }

            // Present the image.
            VkSwapchainKHR swapChains[] = {vulkan->getSwapChain()};
            VkPresentInfoKHR presentInfo = {
                .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                .waitSemaphoreCount = 1,
                .pWaitSemaphores = signalSemaphores,
                .swapchainCount = 1,
                .pSwapchains = swapChains,
                .pImageIndices = &imageIdx,
                .pResults = nullptr
            };
            vkQueuePresentKHR(vulkan->getPresentQueue(), &presentInfo);

            // Updating current frame for correct frame buffer and the command buffer.
            currentFrame = (currentFrame + 1) % maxFramesInFlight;
        }

        void renderer::init()
        {
            std::cout << "Initializing renderer." << std::endl;
            initFrameBuffer();
            initCommandPool();
            initVertexBuffer();
            initCommandBuffers();
            initSyncObjects();
        }
        void renderer::cleanUp()
        {
            std::cout << "Cleaning up renderer." << std::endl;

            std::cout << "\t- Destroying synchronization objects." << std::endl;
            for (size_t i = 0; i < maxFramesInFlight; i++)
            {
                vkDestroyFence(vulkan->getDevice(), inFlightFences[i], nullptr);
                vkDestroySemaphore(vulkan->getDevice(), renderFinishedSemaphores[i], nullptr);
                vkDestroySemaphore(vulkan->getDevice(), imageAvailableSemaphores[i], nullptr);
            }
            
            std::cout << "\t- Destroying vertex buffer." << std::endl;
            vkDestroyBuffer(vulkan->getDevice(), vertexBuffer, nullptr);
            
            std::cout << "\t- Freeing allocated memory." << std::endl;
            vkFreeMemory(vulkan->getDevice(), vertexBufferMemory, nullptr);

            std::cout << "\t- Destroying command pool." << std::endl;
            vkDestroyCommandPool(vulkan->getDevice(), commandPool, nullptr);

            std::cout << "\t- Destroying frame buffers." << std::endl;
            for (VkFramebuffer frameBuffer : frameBuffers)
            {
                vkDestroyFramebuffer(vulkan->getDevice(), frameBuffer, nullptr);
            }
        }
    }
}