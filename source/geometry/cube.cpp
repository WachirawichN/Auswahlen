#include "cube.h"

cube::cube(float width, float height, float depth)
    : width(width), height(height), depth(depth)
{
    vertices = {
    //                   Vertices                   ,   Texture coordinates
        -(width / 2), -(height / 2), -(depth / 2),          0.0f, 0.0f, // Back bottom left
         (width / 2), -(height / 2), -(depth / 2),          1.0f, 0.0f, // Back bottom right
         (width / 2),  (height / 2), -(depth / 2),          1.0f, 1.0f, // Back top right
        -(width / 2),  (height / 2), -(depth / 2),          0.0f, 1.0f, // Back top left
        
        -(width / 2), -(height / 2),  (depth / 2),          0.0f, 0.0f,// Front bottom left
         (width / 2), -(height / 2),  (depth / 2),          1.0f, 0.0f,// Front bottom right
         (width / 2),  (height / 2),  (depth / 2),          1.0f, 1.0f,// Front top right
        -(width / 2),  (height / 2),  (depth / 2),          0.0f, 1.0f,// Front top left
    };
    indices = {
        // Front
        4, 5, 6,
        4, 7, 6,
        // Back
        0, 1, 2,
        0, 3, 2,
        // Left
        0, 3, 7,
        0, 4, 7,
        // Right
        1, 2, 6,
        1, 5, 6,
        // Top
        3,2,6,
        3,7,6,
        // Bottom
        4,0,1,
        4,5,1,
    };
}
std::vector<float> cube::getVertices()
{
    return vertices;
}

std::vector<unsigned int> cube::getIndices()
{
    return indices;
}
