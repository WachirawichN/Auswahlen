#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

namespace auswahlen
{
    namespace graphics
    {
        class window
        {
            private:
                int wWidth = NULL;
                int wHeight = NULL;
                std::string wName;
                GLFWwindow* guiWindow;
            public:
                window() {}
                window(int width, int height, std::string name);
    
                window(const window&) = delete;
                // For copying some basic data before initialization only.
                window& operator=(const window& window);

                GLFWwindow* getWindow() const { return guiWindow; }
                bool shouldClose() const { return glfwWindowShouldClose(guiWindow); }
    
                void createWindow();
                void cleanUp();
        };
    }
}