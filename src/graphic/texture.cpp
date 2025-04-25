#include "texture.h"

texture::texture(const std::string& filePath)
    : textureID(0), filePath(filePath), rawData(nullptr), width(0), height(0), bpp(0)
{
    stbi_set_flip_vertically_on_load(1);
    rawData = stbi_load(filePath.c_str(), &width, &height, &bpp, 4);

    // Need to add a parameter to change rendering mode later
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Horizontal wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Vertical wrap

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawData);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (rawData)
    {
        stbi_image_free(rawData);
    }
}

texture::~texture()
{
    glDeleteTextures(1, &textureID);
}

void texture::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

int texture::getWidth() const
{
    return width;
}

int texture::getheight() const
{
    return height;
}