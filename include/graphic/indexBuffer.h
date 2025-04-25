#pragma once

class indexBuffer
{
    private:
        unsigned int indexBufferID;
        unsigned int totalIndex;
    public:
        indexBuffer(unsigned int count, const void* data);
        ~indexBuffer();

        void bind() const;
        void unbind() const;

        unsigned int getIndexCount() const;
};