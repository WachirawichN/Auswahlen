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
        guiWindow = graphics::window(width, height, "Auswahlen");
        guiWindow.createWindow();
    }
    void app::initVulkan()
    {
        vulkan = graphics::vulkanCore("Auswahlen", {0, 2, 0});
        vulkan.init(guiWindow.getWindow());
    }
    void app::initGraphicPipeline()
    {
        pipeline = graphics::pipeline(&vulkan, "asset/shader/shader.vert.spv", "asset/shader/shader.frag.spv");
        pipeline.init();
    }
    void app::initRenderer()
    {
        renderer = graphics::renderer(&vulkan, &pipeline, vertices);
        renderer.init();
    }

    void app::mainLoop()
    {
        double lastTime = glfwGetTime();
        while (!guiWindow.shouldClose())
        {
            glfwPollEvents();
            renderFrame();
            stepSimulation();

            double currentTime = glfwGetTime();
            double timeDelta = currentTime - lastTime;
            if (DEBUG)
            {
                std::cout << "FPS: " << 1 / timeDelta << std::endl;
            }
            lastTime = currentTime;
        }
        vkDeviceWaitIdle(vulkan.getDevice());
    }
    void app::renderFrame()
    {
        renderer.render();
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
        initRenderer();
        std::cout << "================================================================" << std::endl;
    }
    app::~app()
    {
        std::cout << "================================================================" << std::endl;
        renderer.cleanUp();
        pipeline.cleanUp();
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