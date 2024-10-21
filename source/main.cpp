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

    window = glfwCreateWindow(800, 800, "Auswahlen", NULL, NULL);
    if (!window)
    {
        std::cout << "Fail to create window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
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
        float vertices[] = {
        //        Vertices         ,   Texture coordinates
            -0.5f, -0.5f, -0.5f,         0.0f, 0.0f, // Back bottom left
             0.5f, -0.5f, -0.5f,         1.0f, 0.0f, // Back bottom right
             0.5f,  0.5f, -0.5f,         1.0f, 1.0f, // Back top right
            -0.5f,  0.5f, -0.5f,         0.0f, 1.0f, // Back top left

            -0.5f, -0.5f,  0.5f,         0.0f, 0.0f, // Front bottom left
             0.5f, -0.5f,  0.5f,         1.0f, 0.0f, // Front bottom right
             0.5f,  0.5f,  0.5f,         1.0f, 1.0f, // Front top right
            -0.5f,  0.5f,  0.5f,         0.0f, 1.0f, // Front top left
        };
    
        unsigned int indices[] = {
            // Front
            4, 5, 6,
            4, 7, 6,

            // Back
            0, 1, 2,
            0, 3, 2,

            // Left
            0, 3, 7,
            0, 4, 7,

            // Right
            1, 2, 6,
            1, 5, 6,

            // Top
            3,2,6,
            3,7,6,

            // Bottom
            4,0,1,
            4,5,1,
        };
    
        vertexArray va;
        vertexBuffer vb(8 * 5, vertices);
        vertexBufferLayout layout;
        layout.addLayout(3); // Vertices layout
        layout.addLayout(2); // Texture layout
        indexBuffer ib(6 * 2 * 3, indices); // Side * Triangle per side * Vertices per triangle
    
        va.addBuffer(vb, layout);
    
        shader yeetShader("graphic/res/shader/3d.shader");
        yeetShader.bind();
        //yeetShader.setUniform4f("uColor", 0.3f, 0.2f, 0.7f, 1.0f);

        texture soulKingTexture("graphic/res/texture/dead.png");
        soulKingTexture.bind();
        yeetShader.setUniform1i("uTexture", 0);

        va.unbind();
        vb.unbind();
        ib.unbind();

        renderer sceneRenderer;

        // Perspective projection
        float fov = 90.0f;
        double currentTime = glfwGetTime();

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)width/(float)height, 0.1f, 100.0f);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.f)); 

        yeetShader.setUniformMat4fv("view", GL_FALSE, view);
        yeetShader.setUniformMat4fv("projection", GL_FALSE, projection);
        yeetShader.setUniformMat4fv("model", GL_FALSE, model);

        yeetShader.unbind();

        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        while (!glfwWindowShouldClose(window))
        {
            sceneRenderer.clearScreen();
    
            // Draw vertices here
            sceneRenderer.drawScreen(va, ib, yeetShader);

            if (glfwGetTime() - currentTime >= 0.01)
            {
                model = glm::rotate(model, glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f)); 
                yeetShader.setUniformMat4fv("model", GL_FALSE, model);
                currentTime = glfwGetTime();
            }

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}