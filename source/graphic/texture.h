#pragma once

#include "../dependencies/stb/stb_image.h"

class texture
{
    public:
        texture();
        ~texture();
    
        void bind();
        void unbind();
};