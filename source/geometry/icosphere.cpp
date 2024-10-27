#include "icosphere.h"

#include <iostream>

icosphere::icosphere(float radius, int subdivision)
    : radius(radius), subdivision(subdivision)
{
    const float horizontalAngle = glm::radians(360.0f / 5); // Angle of each verticies pentagon
    const float verticalAngle = atanf(1.0f / 2); // Angle from center of the icosahedron to each vertices of pentagon
    float startAngle[] = {glm::radians(36.0f), glm::radians(72.0f)};
    int elevationMultiplier = 1;

    // Tip / Bottom vertice
    vertices = {
        0.0f,  radius, 0.0f,        0.0f, 0.0f,
        0.0f, -radius, 0.0f,        1.0f, 1.0f,
    };

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            // X
            vertices.push_back(radius * cosf(verticalAngle) * cosf(startAngle[i]));
            // Y
            vertices.push_back(radius * sinf(verticalAngle) * elevationMultiplier);
            // Z
            vertices.push_back(radius * cosf(verticalAngle) * sinf(startAngle[i]));
            // Texture coordinates
            vertices.push_back(1.0f / 5 * (j + 1));
            vertices.push_back(1.0f / 3 * (i + 1));

            // Indices
            // Connect to tip / bottom vertice
            indices.push_back(i); // Tip / Bottom vertice
            indices.push_back((2 + (i * 5)) + j); // Current vertice
            indices.push_back((2 + (i * 5)) + ((j + 1) % 5)); // Next vertice

            // Connect middle section
            indices.push_back((2 + (i * 5)) + j); // Current vertice
            indices.push_back((2 + (i * 5)) + ((j + 1) % 5)); // Next vertice
            indices.push_back(indices[indices.size() -1 + (!i * - 1)] + (5 * (i * -2 + 1))); // Above / Below vertice
            
            startAngle[i] += horizontalAngle;
        }
        elevationMultiplier = -1;
        std::cout << std::endl;
    }
}

std::vector<float> icosphere::getVertices()
{
    return vertices;
}

std::vector<unsigned int> icosphere::getIndices()
{
    return indices;
}