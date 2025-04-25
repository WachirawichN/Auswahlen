#include "vertexArray.h"
#include "vertexBufferLayout.h"
#include "renderer.h"

vertexArray::vertexArray()
{
    glGenVertexArrays(1, &vertexArrayID);
}

vertexArray::~vertexArray()
{
    glDeleteVertexArrays(1, &vertexArrayID);
}

void vertexArray::addBuffer(const vertexBuffer& vertexBuffer, const vertexBufferLayout& layout)
{
    bind();
    vertexBuffer.bind();
    const std::vector<bufferAttribute> attributes = layout.getAttributes();
    unsigned int offset = 0;
    for (int i = 0; i < attributes.size(); i++)
    {
        const bufferAttribute currentAttribute = attributes[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, currentAttribute.count, currentAttribute.type, currentAttribute.normalize, layout.getStride(), (const void*)offset);
        offset += currentAttribute.count * sizeof(currentAttribute.type);
    }
}

void vertexArray::bind() const
{
    glBindVertexArray(vertexArrayID);
}

void vertexArray::unbind() const
{
    glBindVertexArray(0);
}