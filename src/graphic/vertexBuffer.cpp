#include "graphic/vertexBuffer.h"
#include "graphic/renderer.h"

vertexBuffer::vertexBuffer(unsigned int size, const void* data)
{
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);
}

vertexBuffer::~vertexBuffer()
{
    glDeleteBuffers(1, &vertexBufferID);
}

void vertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
}

void vertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}