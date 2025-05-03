#include <graphic/window.h>

namespace auswahlen
{
    window::window(int width, int height, std::string name)
        : wWidth(width), wHeight(height), wName(name)
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        windowData = glfwCreateWindow(wWidth, wHeight, wName.c_str(), nullptr, nullptr);
    }
    window::~window()
    {
        glfwDestroyWindow(windowData);
        glfwTerminate();
    }
}

