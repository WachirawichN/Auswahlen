#include "dependencies/GLEW/include/GL/glew.h"
#include "dependencies/GLFW/glfw3.h"

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
    
    // Initiate GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Fail to initiate GLEW." << std::endl;
    }

    // Getting error message
    glEnable( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( MessageCallback, 0 );


    // Create object, shader, variable, blah blah blah here
    {
        float vertices[] = {
        //    Vertices   ,   Texture coordinates
            -0.5f, -0.5f,      0.0f, 0.0f,
             0.5f, -0.5f,      1.0f, 0.0f,
             0.5f,  0.5f,      1.0f, 1.0f,
            -0.5f,  0.5f,      0.0f, 1.0f,
        };
    
        unsigned int indices[] = {
            0, 1, 2,
            0, 3, 2
        };
    
        vertexArray va;
        vertexBuffer vb(4 * 4, vertices);
        vertexBufferLayout layout;
        layout.addLayout(2); // Vertices layout
        layout.addLayout(2); // Texture layout
        indexBuffer ib(2 * 3, indices);
    
        va.addBuffer(vb, layout);
    
        shader yeetShader("graphic/res/shader/texture.shader");
        yeetShader.bind();
        //yeetShader.setUniform4f("uColor", 0.3f, 0.2f, 0.7f, 1.0f);

        texture soulKingShader("graphic/res/texture/dead.png");
        soulKingShader.bind();
        yeetShader.setUniform1i("uTexture", 0);

        va.unbind();
        vb.unbind();
        ib.unbind();
        yeetShader.unbind();
    
        renderer sceneRenderer;

        while (!glfwWindowShouldClose(window))
        {
            sceneRenderer.clearScreen();
    
            // Draw vertices here
            yeetShader.bind();
            sceneRenderer.drawScreen(va, ib, yeetShader);
    
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}