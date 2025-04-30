#pragma once

#include <iostream>

#include "renderer.h"
#include "stb/stb_image.h"

class texture
{
    private:
        unsigned int textureID;
        std::string filePath;
        unsigned char* rawData;
        int width, height, bpp;
    public:
        texture(const std::string& filePath);
        ~texture();
    
        void bind(unsigned int slot = 0) const;
        void unbind() const;

        int getWidth() const;
        int getheight() const;
};