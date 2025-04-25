#pragma once

#include "vertexBuffer.h"

class vertexBufferLayout;

class vertexArray
{
    private:
        unsigned int vertexArrayID;
    public:
        vertexArray();
        ~vertexArray();

        void addBuffer(const vertexBuffer& vertexBuffer, const vertexBufferLayout& layout);

        void bind() const;
        void unbind() const;
};