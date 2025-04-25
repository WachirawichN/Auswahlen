#pragma once

class vertexBuffer
{
    private:
        unsigned int vertexBufferID;
    public:
        vertexBuffer(unsigned int size, const void* data);
        ~vertexBuffer();

        void bind() const;
        void unbind() const;
};