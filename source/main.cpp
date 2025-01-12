#include "dependencies/GLEW/include/GL/glew.h"
#include "dependencies/GLFW/glfw3.h"

#include "dependencies/GLM/glm.hpp"
#include "dependencies/GLM/ext.hpp"

#include "graphic/graphic.h"
#include "graphic/geometry/geometry.h"

#include "simulation/simulation.h"

#include "simulation/object/sphere.h"
#include "simulation/object/cube.h"

#include <iostream>
#include <iomanip>

// OpenGL Logging
void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam)
{
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type, severity, message );
}

// Window
int width = 2560;
int height = 1440;

//int width = 1920;
//int height = 1080;

// Simulation
float deltaTime = 0.0f;

bool manualStep = true;
float manualDeltaTime = 0.0f;
float deltaTimeStep = 0.0126f;

// Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
float cameraSpeed = 50.0f;
camera worldCamera(cameraPos, glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, (float)16/(float)9, 0.001f, 100.0f);

// Mouse input
float sensitivity = 0.1f;
float lastXMousePos = width /2;
float lastYMousePos = height /2;
void mouseCallbackHandler(GLFWwindow* window, double xPos, double yPos)
{
    float xDst = xPos - lastXMousePos;
    float yDst = lastYMousePos - yPos;
    worldCamera.rotateCamera(xDst * sensitivity, yDst * sensitivity);
    lastXMousePos = xPos;
    lastYMousePos = yPos;
}

// Keyboard input
void keyboardCallbackHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
            case GLFW_KEY_W:
                worldCamera.move(cameraSpeed, deltaTime);
                break;

            case GLFW_KEY_S:
                worldCamera.move(cameraSpeed, deltaTime, false, true);
                break;

            case GLFW_KEY_A:
                worldCamera.move(cameraSpeed, deltaTime, true, true);
                break;

            case GLFW_KEY_D:
                worldCamera.move(cameraSpeed, deltaTime, true, false);
                break;

            // Chaning camera speed
            case GLFW_KEY_V:
                cameraSpeed += 1.0f;
                std::cout << "Camera speed: " << cameraSpeed << std::endl;
                break;
            case GLFW_KEY_C:
                if (cameraSpeed > 0.0f)
                {
                    cameraSpeed -= 1.0f;
                    std::cout << "Camera speed: " << cameraSpeed << std::endl;
                }
                break;

            // Manual simulation step
            case GLFW_KEY_R:
                manualStep = !manualStep;
                break;
                
            case GLFW_KEY_E:
                if (manualStep) manualDeltaTime = deltaTimeStep;
                break;
            case GLFW_KEY_Q:
                if (manualStep) manualDeltaTime = -deltaTimeStep;
                break;

            case GLFW_KEY_X:
                if (manualStep)
                {
                    deltaTimeStep *= 2;
                    std::cout << "Current delta time step: " << deltaTimeStep << std::endl;
                }
                break;
            case GLFW_KEY_Z:
                if (manualStep)
                {
                    deltaTimeStep /= 2;
                    std::cout << "Current delta time step: " << deltaTimeStep << std::endl;
                }
                break;
        }
    }
}


int main()
{
    GLFWwindow* window;
    if (!glfwInit())
    {
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Auswahlen", glfwGetPrimaryMonitor(), NULL);
    if (!window)
    {
        std::cout << "Fail to create window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    glViewport(0, 0, width, height);
    // Background color
    glClearColor(0.0f, 0.02f, 0.1f, 1.0f);
    
    // Initiate GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Fail to initiate GLEW." << std::endl;
    }

    // Getting error message
    //glEnable(GL_DEBUG_OUTPUT);
    //glDebugMessageCallback(MessageCallback, 0);

    glEnable(GL_DEPTH_TEST);
    std::cout << std::fixed << std::setprecision(9);

    // Create object, shader, variable, blah blah blah here
    {
        // Shader program
        shader yeetShader("../graphic/res/3d.shader");
        yeetShader.bind();

        simulation currentSimulation(&worldCamera, yeetShader, -9.8f);


        // Enclosure
        std::shared_ptr<object::cube> bigBlock(new object::cube(true, true, 1.0f, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(10.0f), glm::vec4(0.62f, 0.66f, 0.74f, 1.0f)));
        currentSimulation.addObject(bigBlock);


        // Physic onject
        std::shared_ptr<object::cube> physBlock0(new object::cube(true, false, 1.0f, glm::vec3(1.0f, 0.5f, 0.5f), glm::vec3(-3.5f, -1.5f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
        currentSimulation.addObject(physBlock0);
        std::shared_ptr<object::cube> physBlock1(new object::cube(true, false, 1.0f, glm::vec3(1.0f, 0.5f, 1.0f), glm::vec3(-1.0f, -1.5f, -1.5f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
        currentSimulation.addObject(physBlock1);
        std::shared_ptr<object::cube> physBlock2(new object::cube(true, false, 1.0f, glm::vec3(-2.0f, 0.0f, 0.1f), glm::vec3(2.5f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
        currentSimulation.addObject(physBlock2);
        std::shared_ptr<object::cube> physBlock3(new object::cube(true, false, 1.0f, glm::vec3(3.0f, 2.0f, -1.0f), glm::vec3(-2.0f, -2.0f, 4.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
        currentSimulation.addObject(physBlock3);
        std::shared_ptr<object::cube> physBlock4(new object::cube(true, false, 1.0f, glm::vec3(1.0f, -2.0f, -3.0f), glm::vec3(3.0f, -4.0f, -3.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec4(0.75f, 0.75f, 0.75f, 1.0f)));
        currentSimulation.addObject(physBlock4);
        
        std::shared_ptr<object::sphere> physBall(new object::sphere(1, true, false, 1.0f, glm::vec3(0.0f, 3.0f, -4.0f), glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)));
        currentSimulation.addObject(physBall);

        // Border
        std::shared_ptr<object::cube> outline0(new object::cube(false, true, 1.0f, glm::vec3(0.0f), glm::vec3(0.0f, -5.0f, -5.0f), glm::vec3(0.0f), glm::vec3(10.0, 0.1, 0.1f), glm::vec4(0.7f, 0.5f, 1.0f, 1.0f)));
        currentSimulation.addObject(outline0);
        std::shared_ptr<object::cube> outline1(new object::cube(false, true, 1.0f, glm::vec3(0.0f), glm::vec3(0.0f, -5.0f, 5.0f), glm::vec3(0.0f), glm::vec3(10.0, 0.1, 0.1f), glm::vec4(0.7f, 0.5f, 1.0f, 1.0f)));
        currentSimulation.addObject(outline1);
        std::shared_ptr<object::cube> outline2(new object::cube(false, true, 1.0f, glm::vec3(0.0f), glm::vec3(-5.0f, -5.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.1, 0.1, 10.0f), glm::vec4(0.7f, 0.5f, 1.0f, 1.0f)));
        currentSimulation.addObject(outline2);
        std::shared_ptr<object::cube> outline3(new object::cube(false, true, 1.0f, glm::vec3(0.0f), glm::vec3(5.0f, -5.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.1, 0.1, 10.0f), glm::vec4(0.7f, 0.5f, 1.0f, 1.0f)));
        currentSimulation.addObject(outline3);

        std::shared_ptr<object::cube> outline4(new object::cube(false, true, 1.0f, glm::vec3(0.0f), glm::vec3(0.0f, 5.0f, -5.0f), glm::vec3(0.0f), glm::vec3(10.0, 0.1, 0.1f), glm::vec4(0.7f, 0.5f, 1.0f, 1.0f)));
        currentSimulation.addObject(outline4);
        std::shared_ptr<object::cube> outline5(new object::cube(false, true, 1.0f, glm::vec3(0.0f), glm::vec3(0.0f, 5.0f, 5.0f), glm::vec3(0.0f), glm::vec3(10.0, 0.1, 0.1f), glm::vec4(0.7f, 0.5f, 1.0f, 1.0f)));
        currentSimulation.addObject(outline5);
        std::shared_ptr<object::cube> outline6(new object::cube(false, true, 1.0f, glm::vec3(0.0f), glm::vec3(-5.0f, 5.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.1, 0.1, 10.0f), glm::vec4(0.7f, 0.5f, 1.0f, 1.0f)));
        currentSimulation.addObject(outline6);
        std::shared_ptr<object::cube> outline7(new object::cube(false, true, 1.0f, glm::vec3(0.0f), glm::vec3(5.0f, 5.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.1, 0.1, 10.0f), glm::vec4(0.7f, 0.5f, 1.0f, 1.0f)));
        currentSimulation.addObject(outline7);

        std::shared_ptr<object::cube> outline8(new object::cube(false, true, 1.0f, glm::vec3(0.0f), glm::vec3(-5.0f, 0.0f, -5.0f), glm::vec3(0.0f), glm::vec3(0.1, 10.0, 0.1f), glm::vec4(0.7f, 0.5f, 1.0f, 1.0f)));
        currentSimulation.addObject(outline8);
        std::shared_ptr<object::cube> outline9(new object::cube(false, true, 1.0f, glm::vec3(0.0f), glm::vec3(-5.0f, 0.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.1, 10.0, 0.1f), glm::vec4(0.7f, 0.5f, 1.0f, 1.0f)));
        currentSimulation.addObject(outline9);
        std::shared_ptr<object::cube> outline10(new object::cube(false, true, 1.0f, glm::vec3(0.0f), glm::vec3(5.0f, 0.0f, -5.0f), glm::vec3(0.0f), glm::vec3(0.1, 10.0, 0.1f), glm::vec4(0.7f, 0.5f, 1.0f, 1.0f)));
        currentSimulation.addObject(outline10);
        std::shared_ptr<object::cube> outline11(new object::cube(false, true, 1.0f, glm::vec3(0.0f), glm::vec3(5.0f, 0.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.1, 10.0, 0.1f), glm::vec4(0.7f, 0.5f, 1.0f, 1.0f)));
        currentSimulation.addObject(outline11);


        // Mouse input
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouseCallbackHandler);

        // Keyborad input
        glfwSetKeyCallback(window, keyboardCallbackHandler);

        // Toggle wireframe
        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        
        float lastFrame = glfwGetTime();
        while (!glfwWindowShouldClose(window))
        {
            
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            if (manualStep)
            {
                if (manualDeltaTime != 0.0f)
                {
                    currentSimulation.updateSimulation(manualDeltaTime);
                    manualDeltaTime = 0.0f;
                }
            }
            else
            {
                std::cout << "Delta time: " << deltaTime << std::endl;
                currentSimulation.updateSimulation(deltaTime);
            }
            currentSimulation.drawSimulation();

            //std::cout << "FPS: " << std::round(floor(1.0f / deltaTime)) << std::endl;
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}