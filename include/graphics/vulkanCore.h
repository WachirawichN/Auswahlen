/*

    Class that contains multiple variables that are necessary for writing images onto screen, like image views, swap chain.

*/
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstdint>
#include <string>
#include <array>
#include <optional>
#include <vector>
#include <set>

#include <limits>
#include <algorithm>

// Debug / Error message
#include <iostream>
#include <stdexcept>

#ifndef DEBUG
    #ifdef NDEBUG
        #define DEBUG false
    #else
        #define DEBUG true
    #endif
#endif

namespace auswahlen
{
    namespace graphics
    {
        /*------------------------------------------------------------
            Queue family.
            For storing index of required queue family.

            Each member variable is index of that queue family correspond to the index of the element of "pQueueFamilyProperties",
            this index is from "vkGetPhysicalDeviceQueueFamilyProperties", which is inside "checkCommandSupport" function.
        ------------------------------------------------------------*/
        struct queueFamily {
            std::optional<uint32_t> graphicFamilyIdx;
            std::optional<uint32_t> presentFamilyIdx;

            bool isComplete()
            {
                return graphicFamilyIdx.has_value() && presentFamilyIdx.has_value();
            }
        };
        /*------------------------------------------------------------
            Swap chain supported properties.
            For storing detail about swap chain's supported properties.
        ------------------------------------------------------------*/
        struct swapChainSupportedProperties {
            VkSurfaceCapabilitiesKHR capability;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        class vulkanCore
        {
            private:
                /*------------------------------------------------------------
                    App info.
                ------------------------------------------------------------*/
                std::optional<std::string> name;
                std::optional<std::array<uint32_t, 3>> version;

                /*------------------------------------------------------------
                    Vulkan variables.
                ------------------------------------------------------------*/
                VkInstance instance = VK_NULL_HANDLE;
                VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;

                // Presentation stuffs.
                VkSurfaceKHR surface;
                VkSwapchainKHR swapChain;
                std::vector<VkImage> swapChainImages;
                std::vector<VkImageView> swapChainImageViews;
                VkFormat imageFormat;
                VkExtent2D imageExtent;

                const std::vector<const char*> validationLayers = {
                    "VK_LAYER_KHRONOS_validation"
                };
                const std::vector<const char*> vulkanExtensions = {
                    #if DEBUG
                        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
                    #endif
                };
                const std::vector<const char*> deviceExtensions = {
                    // Optional, if the presentation queue is supported, then the swapchain must be supported too.
                    VK_KHR_SWAPCHAIN_EXTENSION_NAME
                };
                
                // Device handler.
                VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
                VkDevice device;

                // Queue family indices / handlers.
                queueFamily queueFamilyIndices;
                VkQueue graphicQueue;
                VkQueue presentQueue;

                /*------------------------------------------------------------
                    Helper funcions.
                ------------------------------------------------------------*/
                // Initializer functions.
                void initInstance();
                void initDebugCallback();
                void initSurface(GLFWwindow* window);
                void pickPhysicalDevice();
                void initLogicalDevice();
                void initSwapChain(GLFWwindow* window);
                void initImageViews();

                // Debugger functions.
                VkDebugUtilsMessengerCreateInfoEXT populateDebugCreateInfo();
                bool checkValidationLayerSupport();

                static const std::string decodeDebugServerity(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity);
                static const std::string decodeDebugType(VkDebugUtilsMessageTypeFlagsEXT messageType);
                static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
                VkResult createDebugCallback(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator);
                void cleanUpDebugCallback(const VkAllocationCallbacks* pAllocator);

                // Device functions.
                bool isDeviceSuitable(const VkPhysicalDevice& physDevice);
                const queueFamily checkCommandSupport(const VkPhysicalDevice& physDevice);
                bool checkDeviceExtensionsSupport(const VkPhysicalDevice& physDevice);
                const VkDeviceQueueCreateInfo generateQueueCreateInfo(uint32_t queueIdx, uint32_t queueCount, float priority);

                // Swap chain functions.
                const swapChainSupportedProperties querySwapChainSupport(const VkPhysicalDevice& physDevice);
                const VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> availableFormats);
                const VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR> availableModes);
                const VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capability, GLFWwindow* window);
            public:
                vulkanCore() {}
                vulkanCore(const std::string& appName, const std::array<uint32_t, 3>& appVersion);

                vulkanCore(const vulkanCore&) = delete;
                // For copying some basic data before initialization only.
                vulkanCore& operator=(const vulkanCore& vulkan);

                const std::optional<std::string>& getName() const { return name; }
                const std::optional<std::array<uint32_t, 3>>& getAppVersion() const { return version; }
                const VkDevice& getDevice() const { return device; }
                const VkFormat& getFormat() const { return imageFormat; }
                const VkExtent2D& getImgExtent() const { return imageExtent; }
                const queueFamily& getQueueFamilyIndices() const { return queueFamilyIndices; }

                void init(GLFWwindow* window);
                void cleanUp();
        };
    }
}