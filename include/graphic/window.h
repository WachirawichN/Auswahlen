#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

namespace auswahlen
{
    namespace graphic
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
    
                // Remove copy constructor, becuase why not. Who even use this?
                window(const window&) = delete;
                // Repurpose "=" operator into copying width and height of the other window then create a new one.
                window& operator=(const window& window);
    
                int getWidth() const { return wWidth; }
                int getHeight() const { return wHeight; }
                const std::string& getName() const { return wName; }
                bool shouldClose() const { return glfwWindowShouldClose(guiWindow); }
    
                void createWindow();
                void cleanUp();
        };
    }
}