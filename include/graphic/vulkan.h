#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <array>
#include <optional>

#include <iostream>
#include <stdexcept>

namespace auswahlen
{
    namespace graphic
    {
        class vulkan
        {
            private:
                std::optional<std::string> name;
                std::optional<std::array<uint32_t, 3>> version;
                VkInstance instance = VK_NULL_HANDLE;

                // Helper functions
                void initInstance();
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