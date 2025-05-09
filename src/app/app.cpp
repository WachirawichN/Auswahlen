#include <app/app.h>

namespace auswahlen
{
    // Helper functions.
    void app::initGLFW()
    {
        std::cout << "Initializing GLFW." << std::endl;

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        
        std::cout << "\t- Initialization completed." << std::endl;
    }
    void app::initWindow()
    {
        guiWindow = auswahlen::graphic::window(width, height, "Auswahlen");
        guiWindow.createWindow();
    }
    void app::initVulkan()
    {
        vulkan = auswahlen::graphic::vulkan("Auswahlen", {0, 2, 0});
        vulkan.init();
    }

    void app::mainLoop()
    {
        while (!guiWindow.shouldClose())
        {
            glfwPollEvents();
            renderFrame();
            stepSimulation();
        }
    }
    void app::renderFrame()
    {

    }
    void app::stepSimulation()
    {

    }

    void app::run()
    {
        mainLoop();
    }

    // Constructor / Deconstructor.
    app::app(int width, int height)
        : width(width), height(height)
    {
        initGLFW();
        initWindow();
        initVulkan();
        std::cout << "================================================================" << std::endl;
    }
    app::~app()
    {
        std::cout << "================================================================" << std::endl;
        vulkan.cleanUp();
        guiWindow.cleanUp();

        std::cout << "Cleaning up GLFW." << std::endl;
        glfwTerminate();
    }
}