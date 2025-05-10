#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <array>
#include <optional>
#include <vector>

// Debug / Error message
#include <iostream>
#include <stdexcept>

namespace auswahlen
{
    namespace graphic
    {
        class vulkan
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
                const std::vector<const char*> validationLayers = {
                    "VK_LAYER_KHRONOS_validation"
                };
                const std::vector<const char*> vulkanExtensions = {
                    VK_EXT_DEBUG_UTILS_EXTENSION_NAME
                };
                VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
                VkDevice device;

                // Queue family handlers.
                VkQueue graphicQueue;

                /*------------------------------------------------------------
                    Queue family.
                    For checking if all the commands we wanted to use are supported by the queue family that is supported by the device.

                    Each member variable is index of that queue family correspond to the index of the element of "pQueueFamilyProperties",
                    this index is from "vkGetPhysicalDeviceQueueFamilyProperties", which is inside "checkCommandSupport" function.
                ------------------------------------------------------------*/
                struct queueFamily{
                    std::optional<uint32_t> graphicFamily;

                    bool isComplete()
                    {
                        return graphicFamily.has_value();
                    }
                };

                /*------------------------------------------------------------
                    Helper funcions.
                ------------------------------------------------------------*/
                // Initializer functions.
                void initInstance();
                void initDebugCallback();
                void pickPhysicalDevice();
                void initLogicalDevice();

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
                VkDeviceQueueCreateInfo generateQueueCreateInfo(uint32_t queueIdx, uint32_t queueCount, float priority);
            public:
                vulkan() {}
                vulkan(const std::string& appName, const std::array<uint32_t, 3>& appVersion);

                vulkan(const vulkan&) = delete;
                vulkan& operator=(const vulkan& vulkan);

                const std::optional<std::string>& getName() const { return name; }
                const std::optional<std::array<uint32_t, 3>>& getAppVersion() const { return version; }

                void init();
                void cleanUp();
        };
    }
}