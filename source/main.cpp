#include "dependencies/GLEW/include/GL/glew.h"
#include "dependencies/GLFW/glfw3.h"

#include "dependencies/GLM/glm.hpp"
#include "dependencies/GLM/ext.hpp"
#include "dependencies/GLM/matrix.hpp"
#include "dependencies/GLM/vec4.hpp"

#include <iostream>

#include "graphic/renderer.h"
#include "graphic/vertexArray.h"
#include "graphic/vertexBuffer.h"
#include "graphic/vertexBufferLayout.h"
#include "graphic/indexBuffer.h"
#include "graphic/shader.h"
#include "graphic/texture.h"

#include "geometry/cube.h"
#include "geometry/pyramid.h"


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

// Timing
float lastFrame = 0.0f;
float deltaTime = 0.0f;

// Camera
float yaw = -90.0f;
float pitch = 0.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 10.0f;

// Mouse input
float sensitivity = 0.25f;
float lastXMousePos = width /2;
float lastYMousePos = height /2;
void mouseCallbackHandler(GLFWwindow* window, double xPos, double yPos)
{
    float xDst = xPos - lastXMousePos;
    float yDst = lastYMousePos - yPos;
    lastXMousePos = xPos;
    lastYMousePos = yPos;

    yaw += xDst * sensitivity;
    pitch += yDst * sensitivity;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    else if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
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
                cameraPos += cameraFront * cameraSpeed * deltaTime;
                break;

            case GLFW_KEY_S:
                cameraPos -= cameraFront * cameraSpeed * deltaTime;
                break;

            case GLFW_KEY_A:
                cameraPos -= glm::normalize(glm::cross(cameraFront, up)) * cameraSpeed * deltaTime;
                break;

            case GLFW_KEY_D:
                cameraPos += glm::normalize(glm::cross(cameraFront, up)) * cameraSpeed * deltaTime;
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
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    glEnable(GL_DEPTH_TEST); 


    // Create object, shader, variable, blah blah blah here
    {

        pyramid illuminati(1.0f, 1.0f, 1.0f);
        std::vector<float> vertices = illuminati.getVertices();
        std::vector<unsigned int> indices = illuminati.getIndices();

        glm::vec3 illuminatiPositions[] = {
        //              Position              ,             Scale
            glm::vec3(0.0f,   0.5f,    0.0f),   glm::vec3(1.0f,  1.0f,  1.0f),
            glm::vec3(0.0f,  -0.5f,    0.0f),   glm::vec3(1.0f,  1.0f,  1.0f),



            glm::vec3(2.5f,   2.0f,    -2.5f),   glm::vec3(0.8f,  2.0f,  0.8f),
            glm::vec3(2.5f,  -2.0f,    -2.5f),   glm::vec3(0.8f,  2.0f,  0.8f),
            glm::vec3(2.5f,   1.25f,   -2.5f),   glm::vec3(0.75f, 0.75f, 0.75f),
            glm::vec3(2.5f,  -1.25f,   -2.5f),   glm::vec3(0.75f, 0.75f, 0.75f),
 
            glm::vec3(2.5f,   0.125f,  -2.5f),   glm::vec3(0.25f, 0.25f, 0.25f),
            glm::vec3(2.5f,  -0.125f,  -2.5f),   glm::vec3(0.25f, 0.25f, 0.25f),


            glm::vec3(-2.5f,  2.0f,    -2.5f),   glm::vec3(0.8f,  2.0f,  0.8f),
            glm::vec3(-2.5f, -2.0f,    -2.5f),   glm::vec3(0.8f,  2.0f,  0.8f),
            glm::vec3(-2.5f,  1.25f,   -2.5f),   glm::vec3(0.75f, 0.75f, 0.75f),
            glm::vec3(-2.5f, -1.25f,   -2.5f),   glm::vec3(0.75f, 0.75f, 0.75f),
 
            glm::vec3(-2.5f,  0.125f,  -2.5f),   glm::vec3(0.25f, 0.25f, 0.25f),
            glm::vec3(-2.5f, -0.125f,  -2.5f),   glm::vec3(0.25f, 0.25f, 0.25f),



            glm::vec3(2.5f,   2.0f,     2.5f),   glm::vec3(0.8f,  2.0f,  0.8f),
            glm::vec3(2.5f,  -2.0f,     2.5f),   glm::vec3(0.8f,  2.0f,  0.8f),
            glm::vec3(2.5f,   1.25f,    2.5f),   glm::vec3(0.75f, 0.75f, 0.75f),
            glm::vec3(2.5f,  -1.25f,    2.5f),   glm::vec3(0.75f, 0.75f, 0.75f),
 
            glm::vec3(2.5f,   0.125f,   2.5f),   glm::vec3(0.25f, 0.25f, 0.25f),
            glm::vec3(2.5f,  -0.125f,   2.5f),   glm::vec3(0.25f, 0.25f, 0.25f),


            glm::vec3(-2.5f,  2.0f,     2.5f),   glm::vec3(0.8f,  2.0f,  0.8f),
            glm::vec3(-2.5f, -2.0f,     2.5f),   glm::vec3(0.8f,  2.0f,  0.8f),
            glm::vec3(-2.5f,  1.25f,    2.5f),   glm::vec3(0.75f, 0.75f, 0.75f),
            glm::vec3(-2.5f, -1.25f,    2.5f),   glm::vec3(0.75f, 0.75f, 0.75f),
 
            glm::vec3(-2.5f,  0.125f,   2.5f),   glm::vec3(0.25f, 0.25f, 0.25f),
            glm::vec3(-2.5f, -0.125f,   2.5f),   glm::vec3(0.25f, 0.25f, 0.25f),
        };

        vertexArray va;
        vertexBuffer vb(vertices.size(), vertices.data());
        vertexBufferLayout layout;
        layout.addLayout(3); // Vertices layout
        layout.addLayout(2); // Texture layout
        indexBuffer ib (indices.size(), indices.data());
    
        va.addBuffer(vb, layout);
    
        shader yeetShader("graphic/res/shader/3d.shader");
        yeetShader.bind();

        texture soulKingTexture("graphic/res/texture/fade.png");
        soulKingTexture.bind();
        yeetShader.setUniform1i("uTexture", 0);

        va.unbind();
        vb.unbind();
        ib.unbind();

        renderer sceneRenderer;

        // Perspective projection
        float fov = 90.0f;

        // Projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)width/(float)height, 0.1f, 100.0f);

        yeetShader.setUniformMat4fv("projection", GL_FALSE, projection);

        // Mouse input
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouseCallbackHandler);

        // Keyborad input
        glfwSetKeyCallback(window, keyboardCallbackHandler);

        // Toggle wireframe
        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        while (!glfwWindowShouldClose(window))
        {
            sceneRenderer.clearScreen();
            float currentFrame = glfwGetTime();
            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, up);
    
            // Draw vertices here
            // Render crystals
            for (int i = 0; i < 26; i++)
            {
                int multiplier = 1; // Define rotation direction
                if (i & 2 > 0.0f)
                {
                    multiplier = -1;
                }
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, illuminatiPositions[i * 2]);
                model = glm::scale(model, illuminatiPositions[(i * 2) + 1]);

                model = glm::rotate(model, glm::radians(i * 180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //Flip pyramid

                // Rotate pyramid
                float rotateAngle = glfwGetTime() * 20.0f;
                model = glm::rotate(model, glm::radians(rotateAngle * multiplier), glm::vec3(0.0f, 1.0f, 0.0f));

                yeetShader.setUniformMat4fv("model", GL_FALSE, model);
                yeetShader.setUniformMat4fv("view", GL_FALSE, view);
                sceneRenderer.drawScreen(va, ib, yeetShader);
            }

            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}