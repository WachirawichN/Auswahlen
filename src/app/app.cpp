#include <app/app.h>

namespace auswahlen
{
    /*------------------------------------------------------------
        Helper funcions.
    ------------------------------------------------------------*/
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
        guiWindow = auswahlen::graphics::window(width, height, "Auswahlen");
        guiWindow.createWindow();
    }
    void app::initVulkan()
    {
        vulkan = auswahlen::graphics::vulkanCore("Auswahlen", {0, 2, 0});
        vulkan.init(guiWindow.getWindow());
    }
    void app::initGraphicPipeline()
    {
        pipeline = auswahlen::graphics::pipeline("asset/shader/shader.vert.spv", "asset/shader/shader.frag.spv");
        pipeline.init(vulkan);
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

    /*------------------------------------------------------------
        Public funcions.
    ------------------------------------------------------------*/
    app::app(int width, int height)
        : width(width), height(height)
    {
        initGLFW();
        initWindow();
        initVulkan();
        initGraphicPipeline();
        std::cout << "================================================================" << std::endl;
    }
    app::~app()
    {
        std::cout << "================================================================" << std::endl;
        pipeline.cleanUp(vulkan);
        vulkan.cleanUp();
        guiWindow.cleanUp();

        std::cout << "Cleaning up GLFW." << std::endl;
        glfwTerminate();
    }

    void app::run()
    {
        mainLoop();
    }
}