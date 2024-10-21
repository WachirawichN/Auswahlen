#include "renderer.h"

void renderer::drawScreen(vertexArray& vertexArray, indexBuffer& indexBuffer, shader& shader) const
{
    shader.bind();
    vertexArray.bind();
    indexBuffer.bind();
    glDrawElements(GL_TRIANGLES, indexBuffer.getIndexCount(), GL_UNSIGNED_INT, nullptr);
}

void renderer::clearScreen() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}