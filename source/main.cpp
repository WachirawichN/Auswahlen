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

// Timing
float lastFrame = 0.0f;
float deltaTime = 0.0f;

// Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
float cameraSpeed = 10.0f;
camera worldCamera(cameraPos);

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
    glClearColor(0.0f, 0.02f, 0.1f, 1.0f);
    
    // Initiate GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Fail to initiate GLEW." << std::endl;
    }

    // Getting error message
    //lEnable(GL_DEBUG_OUTPUT);
    //lDebugMessageCallback(MessageCallback, 0);

    glEnable(GL_DEPTH_TEST);

    // Create object, shader, variable, blah blah blah here
    {
        // Shader program
        shader yeetShader("graphic/res/shader/3d.shader");
        yeetShader.bind();

        // Textures
        texture soulKingTexture("graphic/res/texture/fade.png");
        soulKingTexture.bind();
        yeetShader.setUniform1i("uTexture", 0);

        simulation currentSimulation(&worldCamera, yeetShader);


        
        std::shared_ptr<object::sphere> spherePtr1(new object::sphere(3, true, false, 1.0f, glm::vec3(35.0f, -20.0f, 42.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f)));
        currentSimulation.addObject(spherePtr1);
        std::shared_ptr<object::sphere> spherePtr2(new object::sphere(3, true, false, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 4.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
        currentSimulation.addObject(spherePtr2);
        std::shared_ptr<object::cube> physBlock(new object::cube(true, false, 1.0f, glm::vec3(-2.0f, -4.0f, 7.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f)));
        currentSimulation.addObject(physBlock);
        //std::shared_ptr<object::cube> block(new object::cube(true, true, 99999.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 1.0f, 10.0f)));
        //currentSimulation.addObject(block);
        
        
        /*
        std::shared_ptr<object::sphere> spherePtr1(new object::sphere(3, true, true, 1.0f, glm::vec3(-1.5f, 0.0f, 0.0f), glm::vec3(2.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f)));
        currentSimulation.addObject(spherePtr1);
        std::shared_ptr<object::sphere> spherePtr2(new object::sphere(3, true, true, 1.0f, glm::vec3(1.5f, 0.0f, 0.0f), glm::vec3(-2.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f)));
        currentSimulation.addObject(spherePtr2);
        */

        
        std::shared_ptr<object::cube> topPtr(new object::cube(true, true, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 0.5f, 10.0f)));
        std::shared_ptr<object::cube> floorPtr(new object::cube(true, true, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 0.5f, 10.0f)));

        std::shared_ptr<object::cube> rightPtr(new object::cube(true, true, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 10.0f, 10.0f)));
        std::shared_ptr<object::cube> leftPtr(new object::cube(true, true, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 10.0f, 10.0f)));

        std::shared_ptr<object::cube> frontPtr(new object::cube(true, true, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 0.5f)));
        std::shared_ptr<object::cube> backPtr(new object::cube(true, true, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 0.5f)));


        currentSimulation.addObject(topPtr);
        currentSimulation.addObject(floorPtr);
        currentSimulation.addObject(rightPtr);
        currentSimulation.addObject(leftPtr);
        currentSimulation.addObject(frontPtr);
        currentSimulation.addObject(backPtr);
        
        
        /*
        srand(0);
        for (int i = 0; i < 10; i++)
        {
            glm::vec3 initailVelocity(((rand() & 101) - 50) / 10, ((rand() & 101) - 50) / 50, ((rand() & 101) - 50) / 10);
            glm::vec3 initailPosition(((rand() & 7) - 3), ((rand() & 7) - 3), ((rand() & 7) - 3));
            std::shared_ptr<object::sphere> spherePtr(new object::sphere(3, true, true, 1.0f, initailVelocity, initailPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f)));
            currentSimulation.addObject(spherePtr);
        }
        */
        
        

        // Mouse input
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouseCallbackHandler);

        // Keyborad input
        glfwSetKeyCallback(window, keyboardCallbackHandler);

        // Toggle wireframe
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        // FOR RECODING WITH DEVLOG ONLY (use nvidia replay for best result)
        //float haltCurrentTime = glfwGetTime();
        //while (haltCurrentTime < 5.0f)
        //{
        //    deltaTime = haltCurrentTime - lastFrame;
        //    lastFrame = haltCurrentTime;
        //    haltCurrentTime = glfwGetTime();
        //}
        
        while (!glfwWindowShouldClose(window))
        {
            float currentFrame = glfwGetTime();
            
            currentSimulation.updateSimulation(deltaTime);
            currentSimulation.drawSimulation();

            //std::cout << "FPS: " << floor(1.0f / deltaTime) << std::endl;
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}