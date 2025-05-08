#include <graphic/vulkan.h>

namespace auswahlen
{
    namespace graphic
    {
        // Helper functions
        void vulkan::initInstance()
        {
            if (!(name.has_value() && version.has_value()))
            {
                throw std::runtime_error("Either name or version or both is not available to Vulkan class.");
            }

            // App info
            VkApplicationInfo appInfo{};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = name.value().c_str();
            appInfo.applicationVersion = VK_MAKE_VERSION(version.value()[0], version.value()[1], version.value()[2]);
            appInfo.pEngineName = "No engine";
            appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.apiVersion = VK_API_VERSION_1_4;

            // Create info
            VkInstanceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;

            // Requirement extension
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
            createInfo.enabledExtensionCount = glfwExtensionCount;
            createInfo.ppEnabledExtensionNames = glfwExtensions;

            // Validation layers.
            createInfo.enabledLayerCount = 0;

            // Create the instance itself.
            if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create Vulkan instance.");
            }
            std::cout << "Initialized Vulkan instance." << std::endl;
        }

        // Public functions
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

        void vulkan::init()
        {
            initInstance();
        }
        void vulkan::cleanUp()
        {
            std::cout << "Cleaning up vulkan." << std::endl;
            vkDestroyInstance(instance, nullptr);
        }
    }
}