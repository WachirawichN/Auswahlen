#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace auswahlen
{
    class window
    {
        private:
            const int wWidth;
            const int wHeight;
            std::string wName;
            GLFWwindow* windowData;
        public:
            // Initialize all the GLFW stuff and create window.
            window(int width, int height, std::string name);
            ~window();

            bool shouldClose() { return glfwWindowShouldClose(windowData); }

            // Remove copy constructor/overload
            window(const window&) = delete;
            window& operator=(const window&) = delete;
    };
}