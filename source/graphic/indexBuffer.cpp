#include "renderer.h"
#include "indexBuffer.h"

indexBuffer::indexBuffer(unsigned int count, const void* data)
    : totalIndex(count)
{
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

indexBuffer::~indexBuffer()
{
    glDeleteBuffers(1, &indexBufferID);
}

void indexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
}

void indexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int indexBuffer::getIndexCount() const
{
    return totalIndex;
}