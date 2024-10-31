#include "pyramid.h"

geometry::pyramid::pyramid(float width, float height, float depth)
    : width(width), height(height), depth(depth)
{
    vertices = {
    //                   Vertices                   ,   Texture coordinates
          0,           (height / 2),            0,          0.5f, 0.5f,// Tip

        -(width / 2), -(height / 2), -(depth / 2),          0.0f, 0.0f, // Back bottom left
         (width / 2), -(height / 2), -(depth / 2),          1.0f, 0.0f, // Back bottom right
         (width / 2), -(height / 2),  (depth / 2),          1.0f, 1.0f, // Front bottom right
        -(width / 2), -(height / 2),  (depth / 2),          0.0f, 1.0f, // Front bottom left
    };
    indices = {
        // Base
        1,2,3,
        1,4,3,

        // Front
        0,3,4,
        // Left
        0,1,4,
        // Right
        0,2,3,
        // Back
        0,1,2,
    };
}
std::vector<float> geometry::pyramid::getVertices()
{
    return vertices;
}

std::vector<unsigned int> geometry::pyramid::getIndices()
{
    return indices;
}
