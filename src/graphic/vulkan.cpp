#include <graphic/vulkan.h>

namespace auswahlen
{
    namespace graphic
    {
        /*------------------------------------------------------------
            Helper functions.
        ------------------------------------------------------------*/
        // Initializer functions.
        void vulkan::initInstance()
        {
            std::cout << "\t- Initializing Vulkan instance." << std::endl;

            if (!(name.has_value() && version.has_value()))
            {
                throw std::runtime_error("Either name or version or both is not available to Vulkan class.");
            }

            if (DEBUG && !checkValidationLayerSupport())
            {
                throw std::runtime_error("One of the validation layers is not supported.");
            }

            // App info.
            VkApplicationInfo appInfo = {
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pApplicationName = name.value().c_str(),
                .applicationVersion = VK_MAKE_VERSION(version.value()[0], version.value()[1], version.value()[2]),
                .pEngineName = "No engine",
                .engineVersion = VK_MAKE_VERSION(1, 0, 0),
                .apiVersion = VK_API_VERSION_1_4
            };

            // GLFW's requirement extensions.
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            // Copy GLFW's extensions into final vector.
            std::vector<const char*> finalExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
            // Join GLFW's requirement extensions with Vulkan's extension.
            finalExtensions.insert(finalExtensions.end(), vulkanExtensions.begin(), vulkanExtensions.end());

            // Instance's create info.
            VkInstanceCreateInfo createInfo = {
                .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                .pApplicationInfo = &appInfo,
                .enabledExtensionCount = (uint32_t)finalExtensions.size(),
                .ppEnabledExtensionNames = finalExtensions.data(),
            };

            // Modify instance's create info for validation layer.
            if (DEBUG)
            {
                VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = populateDebugCreateInfo();
                createInfo.pNext = &debugCreateInfo;
                createInfo.enabledLayerCount = validationLayers.size();
                createInfo.ppEnabledLayerNames = validationLayers.data();
            }

            // Create the instance itself.
            if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create Vulkan instance.");
            }
            std::cout << "\t\t- Initialization completed." << std::endl;
        }
        void vulkan::initDebugCallback()
        {
            std::cout << "\t- Initializing debug callback." << std::endl;
            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = populateDebugCreateInfo();
            if (createDebugCallback(&debugCreateInfo, nullptr) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to initialize debug callback.");
            }
            std::cout << "\t\t- Initialization completed." << std::endl;
        }
        void vulkan::initSurface(GLFWwindow* window)
        {
            std::cout << "\t- Initializing window surface." << std::endl;
            if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
            {
                std::runtime_error("\t\t- Failed to create window surface.");
            }
            std::cout << "\t\t- Initialization completed." << std::endl;
        }
        void vulkan::pickPhysicalDevice()
        {
            std::cout << "\t- Picking physical device." << std::endl;

            // Get total devices with Vulkan support.
            uint32_t deviceCount;
            vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
            if (deviceCount == 0)
            {
                std::runtime_error("No device with Vulkan support found.");
            }

            // Get devices with Vulkan support.
            std::vector<VkPhysicalDevice> devicesList(deviceCount);
            vkEnumeratePhysicalDevices(instance, &deviceCount, devicesList.data());

            // Check for devices that is suitable.
            //std::vector<VkPhysicalDevice> suitableDevices;
            for (const VkPhysicalDevice& physDevice : devicesList)
            {
                if (isDeviceSuitable(physDevice))
                {
                    physicalDevice = physDevice;
                    break;
                }
            }

            if (physicalDevice == VK_NULL_HANDLE)
            {
                std::runtime_error("Cannot find sutable device.");
            }

            // Pick best device.

            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(physicalDevice, &properties);
            std::cout << "\t\t- Pick " << properties.deviceName << " as a physical device." << std::endl;
        }
        void vulkan::initLogicalDevice()
        {
            std::cout << "\t- Initializing logical device." << std::endl;

            // Create queue create info for all queue families we wanted.
            vulkan::queueFamily indices = checkCommandSupport(physicalDevice);
            std::set<uint32_t> uniqueQueueFamilyIdx = {
                indices.graphicFamilyIdx.value(),
                indices.presentFamilyIdx.value()
            };
            std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

            for (uint32_t queueFamilyIdx : uniqueQueueFamilyIdx)
            {
                VkDeviceQueueCreateInfo queueCreateInfo = generateQueueCreateInfo(queueFamilyIdx, 1, 1.0f);
                queueCreateInfos.push_back(queueCreateInfo);
            }

            // Device's create info.
            VkPhysicalDeviceFeatures deviceFeatures{};
            VkDeviceCreateInfo deviceCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                .queueCreateInfoCount = (uint32_t)queueCreateInfos.size(),
                .pQueueCreateInfos = queueCreateInfos.data(),
                .enabledExtensionCount = (uint32_t)deviceExtensions.size(),
                .ppEnabledExtensionNames = deviceExtensions.data(),
                .pEnabledFeatures = &deviceFeatures
            };

            // Create device itself.
            if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS)
            {
                std::runtime_error("Failed to create logical device.");
            }

            // Assigning queue that have been create along the logical device to each queue's handler.
            vkGetDeviceQueue(device, indices.graphicFamilyIdx.value(), 0, &graphicQueue);
            vkGetDeviceQueue(device, indices.presentFamilyIdx.value(), 0, &presentQueue);

            std::cout << "\t\t- Initialization completed." << std::endl;
        }
        void vulkan::initSwapChain(GLFWwindow* window)
        {
            std::cout << "\t- Initializing swap chain." << std::endl;

            // Swap chain properties.
            swapChainSupportedProperties swapChainSupportInfo = querySwapChainSupport(physicalDevice);
            const VkSurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(swapChainSupportInfo.formats);
            const VkPresentModeKHR presentMode = choosePresentMode(swapChainSupportInfo.presentModes);
            const VkExtent2D extent = chooseSwapExtent(swapChainSupportInfo.capability, window);

            // Recommended to have one more than the minimum.
            uint32_t imageCounts = swapChainSupportInfo.capability.minImageCount + 1;

            // Making sure that the image counts doesn't exceed that maximum that is allowed.
            if (imageCounts > swapChainSupportInfo.capability.maxImageCount && swapChainSupportInfo.capability.maxImageCount > 0)
            {
                imageCounts = swapChainSupportInfo.capability.maxImageCount;
            }

            // Swap chain's create info.
            VkSwapchainCreateInfoKHR createInfo = {
                .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                .surface = surface,
                .minImageCount = imageCounts,
                .imageFormat = surfaceFormat.format,
                .imageColorSpace = surfaceFormat.colorSpace,
                .imageExtent = extent,
                .imageArrayLayers = 1,
                .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                .preTransform = swapChainSupportInfo.capability.currentTransform,
                .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                .presentMode = presentMode,
                .clipped = VK_TRUE
            };

            // Check if the graphic queue and the presentation queue is from the same family.
            queueFamily indices = checkCommandSupport(physicalDevice);
            if (indices.graphicFamilyIdx.value() != indices.presentFamilyIdx.value())
            {
                uint32_t queueFamilyindicesArray[] = {indices.graphicFamilyIdx.value(), indices.presentFamilyIdx.value()};

                // Can use VK_SHARING_MODE_EXCLUSIVE instead, but required more setup.
                createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = queueFamilyindicesArray;
            }
            else
            {
                createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            }

            // Create the swap chain itself.
            if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
            {
                std::runtime_error("Failed to initialize swap chain.");
            }
            std::cout << "\t\t- Initialization completed." << std::endl;

            // Get the swap chain's images handler.
            vkGetSwapchainImagesKHR(device, swapChain, &imageCounts, nullptr);
            swapChainImages.resize(imageCounts);
            vkGetSwapchainImagesKHR(device, swapChain, &imageCounts, swapChainImages.data());

            // For use with graphics pipeline.
            imageFormat = surfaceFormat.format;
            imageExtent = extent;
        }
        void vulkan::initImageViews()
        {
            std::cout << "\t- Initializing image views." << std::endl;

            swapChainImageViews.resize(swapChainImages.size());

            // Specify component mapping and subresource range for all image views.
            VkComponentMapping componentMapping = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            };
            VkImageSubresourceRange subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            };

            // Create view for every image.
            for (int i = 0; i < swapChainImages.size(); i++)
            {
                VkImageViewCreateInfo createInfo = {
                    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                    .image = swapChainImages[i],
                    .viewType = VK_IMAGE_VIEW_TYPE_2D,
                    .format = imageFormat,
                    .components = componentMapping,
                    .subresourceRange = subresourceRange
                };

                if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
                {
                    std::runtime_error("Failed to initialize image views.");
                }
            }
            std::cout << "\t\t- Initialization completed." << std::endl;
        }

        // Debugger functions.
        VkDebugUtilsMessengerCreateInfoEXT vulkan::populateDebugCreateInfo()
        {
            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                .messageSeverity =
                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                    | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                .messageType =
                    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                    | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                    | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                .pfnUserCallback = debugCallback
            };
            return debugCreateInfo;
        }
        bool vulkan::checkValidationLayerSupport()
        {
            // Total validation layers.
            uint32_t totalLayers;
            vkEnumerateInstanceLayerProperties(&totalLayers, nullptr);

            // Actual layers itself.
            std::vector<VkLayerProperties> availableLayers(totalLayers);
            vkEnumerateInstanceLayerProperties(&totalLayers, availableLayers.data());

            // Check if layers we want to use is inside the layers that are available vector.
            for (const char* layer : validationLayers)
            {
                bool found = false;

                // Compare the name of layer we wanted to check with every available layer's name.
                for (VkLayerProperties layerProperties : availableLayers)
                {
                    if (strcmp(layer, layerProperties.layerName) == 0)
                    {
                        found = true;
                        break;
                    }
                }

                // Did not find the layer in available layers.
                if (!found)
                {
                    return false;
                }
            }

            // Did find all the layer we wanted to use in available layers.
            return true;
        }
        
        const std::string vulkan::decodeDebugServerity(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity)
        {
            switch (messageSeverity)
            {
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                    return "Verbose.";
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                    return "Info.";
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                    return "Warning.";
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                    return "Error.";
                default:
                    return "UNKNOWN SERVERITY.";
            }
        }
        const std::string vulkan::decodeDebugType(VkDebugUtilsMessageTypeFlagsEXT messageType)
        {
            switch (messageType)
            {
                case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
                    return "General.";
                case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
                    return "Validation.";
                case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
                    return "Performance.";
                default:
                    return "UNKNOWN TYPE.";
            }
        }
        VKAPI_ATTR VkBool32 VKAPI_CALL vulkan::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
        {
            std::cout << "Debug callback: " << pCallbackData->pMessage << std::endl;
            std::cout << "\t- Serverity: " << decodeDebugServerity(messageSeverity) << std::endl;
            std::cout << "\t- Type: " << decodeDebugType(messageType) << std::endl;
            return VK_FALSE;
        }
        VkResult vulkan::createDebugCallback(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator)
        {
            // Find address of a function that create debug callback (Because it's an extension function).
            PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
            if (func)
            {
                return func(instance, pCreateInfo, pAllocator, &debugMessenger);
            }

            // Cannot find extension
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
        void vulkan::cleanUpDebugCallback(const VkAllocationCallbacks* pAllocator)
        {
            // Find address of a function that delete debug callback (Because it's an extension function).
            PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
            if (func)
            {
                func(instance, debugMessenger, pAllocator);
            }
        }

        // Device functions.
        bool vulkan::isDeviceSuitable(const VkPhysicalDevice& physDevice)
        {
            vulkan::queueFamily indicies = checkCommandSupport(physDevice);
            bool supportDeviceExtensions = checkDeviceExtensionsSupport(physDevice);

            // Check if swap chain that we've supported surface formats and presentation modes.
            bool swapChainSuitable = false;
            if (supportDeviceExtensions)
            {
                swapChainSupportedProperties swapChainSupportInfo = querySwapChainSupport(physDevice);
                swapChainSuitable = !swapChainSupportInfo.formats.empty() && !swapChainSupportInfo.presentModes.empty();
            }

            return indicies.isComplete() && supportDeviceExtensions && swapChainSuitable;
        }
        const vulkan::queueFamily vulkan::checkCommandSupport(const VkPhysicalDevice& physDevice)
        {
            vulkan::queueFamily indices;

            // Get all the queue family that are supported.
            uint32_t supportedQueueFamiliesCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &supportedQueueFamiliesCount, nullptr);
            
            std::vector<VkQueueFamilyProperties> supportedQueueFamilies(supportedQueueFamiliesCount);
            vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &supportedQueueFamiliesCount, supportedQueueFamilies.data());

            // Check if any of those supported queue family support the commands we want,
            // and assigning corresponding index to those queue family member variable.
            int i = 0;
            for (VkQueueFamilyProperties queueFamily : supportedQueueFamilies)
            {
                VkBool32 surfaceSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(physDevice, i, surface, &surfaceSupport);
                if (surfaceSupport)
                {
                    indices.presentFamilyIdx = i;
                }
                if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    indices.graphicFamilyIdx = i;
                }

                if (indices.isComplete())
                {
                    break;
                }
                i++;
            }

            return indices;
        }
        bool vulkan::checkDeviceExtensionsSupport(const VkPhysicalDevice& physDevice)
        {
            // Get extensions that are supported by the device.
            uint32_t supportedExtensionsCount;
            vkEnumerateDeviceExtensionProperties(physDevice, nullptr, &supportedExtensionsCount, nullptr);

            std::vector<VkExtensionProperties> supportedExtensions(supportedExtensionsCount);
            vkEnumerateDeviceExtensionProperties(physDevice, nullptr, &supportedExtensionsCount, supportedExtensions.data());

            // Remove the required extension from a set, if the device supported that extension.
            // The set must be a set of string to avoid it not recognize it as the same value.
            std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
            for (VkExtensionProperties supportedExtension : supportedExtensions)
            {
                requiredExtensions.erase(supportedExtension.extensionName);
            }

            return requiredExtensions.empty();
        }
        const VkDeviceQueueCreateInfo vulkan::generateQueueCreateInfo(const uint32_t queueIdx, const uint32_t queueCount, const float priority)
        {
            VkDeviceQueueCreateInfo queueCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = queueIdx,
                .queueCount = queueCount,
                .pQueuePriorities = &priority
            };
            return queueCreateInfo;
        }

        // Swap chain functions.
        const vulkan::swapChainSupportedProperties vulkan::querySwapChainSupport(const VkPhysicalDevice& physDevice)
        {
            swapChainSupportedProperties supportInfo;

            // Capability.
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDevice, surface, &supportInfo.capability);

            // Formats.
            uint32_t formatsCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatsCount, nullptr);
            if (formatsCount != 0)
            {
                supportInfo.formats.resize(formatsCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatsCount, supportInfo.formats.data());
            }
            
            // Present modes.
            uint32_t presentModesCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presentModesCount, nullptr);
            if (presentModesCount != 0)
            {
                supportInfo.presentModes.resize(presentModesCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presentModesCount, supportInfo.presentModes.data());
            }

            return supportInfo;
        }
        const VkSurfaceFormatKHR vulkan::chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> availableFormats)
        {
            for (VkSurfaceFormatKHR surfaceFormat : availableFormats)
            {
                if (surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR  && surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB)
                {
                    return surfaceFormat;
                }
            }
            return availableFormats[0];
        }
        const VkPresentModeKHR vulkan::choosePresentMode(const std::vector<VkPresentModeKHR> availableModes)
        {
            for (VkPresentModeKHR presentMode : availableModes)
            {
                if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                {
                    return presentMode;
                }
            }
            return VK_PRESENT_MODE_FIFO_KHR;
        }
        const VkExtent2D vulkan::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capability, GLFWwindow* window)
        {
            // Check if the current capability's resolution doesn't surpass the largest number of uint32_t type.
            // Else then compute the new resolution that is within the min-max of supported extents.
            if (capability.currentExtent.width != std::numeric_limits<uint32_t>::max())
            {
                return capability.currentExtent;
            }
            else
            {
                int width, height;
                glfwGetFramebufferSize(window, &width, &height);

                VkExtent2D actualExtent = {
                    (uint32_t)width,
                    (uint32_t)height,
                };

                actualExtent.width = std::clamp(actualExtent.width, capability.minImageExtent.width, capability.maxImageExtent.width);
                actualExtent.height = std::clamp(actualExtent.height, capability.minImageExtent.height, capability.maxImageExtent.height);
                return actualExtent;
            }
        }

        /*------------------------------------------------------------
            Public functions.
        ------------------------------------------------------------*/
        vulkan::vulkan(const std::string& appName, const std::array<uint32_t, 3>& appVersion)
            : name{appName}, version{appVersion}
        {
        }

        vulkan& vulkan::operator=(const vulkan& vulkan)
        {
            // Check if the Vulkan class value is not set yet, and the other Vulkan did have value, and check if tried to perform operator on itself.
            if (!(name.has_value() || version.has_value()) && (vulkan.getName().has_value() && vulkan.getAppVersion().has_value()) && (this != &vulkan))
            {
                name = vulkan.getName();
                version = vulkan.getAppVersion();
            }
            return *this;
        }

        void vulkan::init(GLFWwindow* window)
        {
            std::cout << "Initializing Vulkan." << std::endl;
            initInstance();
            if (DEBUG)
            {
                initDebugCallback();
            }
            initSurface(window);
            pickPhysicalDevice();
            initLogicalDevice();
            initSwapChain(window);
            initImageViews();
        }
        void vulkan::cleanUp()
        {
            std::cout << "Cleaning up vulkan." << std::endl;

            std::cout << "\t- Destroying image views." << std::endl;
            for (VkImageView imageView : swapChainImageViews)
            {
                vkDestroyImageView(device, imageView, nullptr);
            }

            std::cout << "\t- Destroying swap chain." << std::endl;
            vkDestroySwapchainKHR(device, swapChain, nullptr);

            std::cout << "\t- Destroying logical device." << std::endl;
            vkDestroyDevice(device, nullptr);

            std::cout << "\t- Destroying window surface." << std::endl;
            vkDestroySurfaceKHR(instance, surface, nullptr);
            
            if (DEBUG)
            {
                std::cout << "\t- Destroying debug messenger." << std::endl;
                cleanUpDebugCallback(nullptr);
            }            

            std::cout << "\t- Destroying Vulkan instance." << std::endl;
            vkDestroyInstance(instance, nullptr);
        }
    }
}