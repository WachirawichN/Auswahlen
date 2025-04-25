#include "vertexBufferLayout.h"

vertexBufferLayout::vertexBufferLayout()
    : stride(0)
{}

void vertexBufferLayout::addLayout(unsigned int count)
{
    bufferAttributes.push_back({GL_FLOAT, count, GL_FALSE});
    stride += count * sizeof(GL_FLOAT);
}

std::vector<bufferAttribute> vertexBufferLayout::getAttributes() const
{
    return bufferAttributes;
}

unsigned int vertexBufferLayout::getStride() const
{
    return stride;
}
