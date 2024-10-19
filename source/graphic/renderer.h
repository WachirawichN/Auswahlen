#pragma once

#include "../dependencies/GLEW/include/GL/glew.h"

#include "vertexArray.h"
#include "indexBuffer.h"
#include "shader.h"

class renderer
{
    public:
        void drawScreen(vertexArray& vertexArray, indexBuffer& indexBuffer, shader& shader) const;
        void clearScreen() const;
};