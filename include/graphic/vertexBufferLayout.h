#pragma once

#include <vector>
#include "renderer.h"

struct bufferAttribute
{
    unsigned int type;
    unsigned int count;
    GLboolean normalize;
};


class vertexBufferLayout
{
    private:
        unsigned int stride;
        std::vector<bufferAttribute> bufferAttributes;
    public:
        vertexBufferLayout();

        void addLayout(unsigned int count);

        std::vector<bufferAttribute> getAttributes() const;
        unsigned int getStride() const;
};
