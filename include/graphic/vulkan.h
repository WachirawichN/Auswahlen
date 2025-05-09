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

                /*------------------------------------------------------------
                    Helper funcions.
                ------------------------------------------------------------*/
                // Initializer functions.
                void initInstance();
                void initDebugCallback();

                // Debugger functions
                VkDebugUtilsMessengerCreateInfoEXT populateDebugCreateInfo();
                bool checkValidationLayerSupport();

                static const std::string decodeDebugServerity(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity);
                static const std::string decodeDebugType(VkDebugUtilsMessageTypeFlagsEXT messageType);
                static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
                VkResult createDebugCallback(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator);
                void cleanUpDebugCallback(const VkAllocationCallbacks* pAllocator);
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