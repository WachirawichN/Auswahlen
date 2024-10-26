#include "icosphere.h"

#include <iostream>

icosphere::icosphere(float radius, int subdivision)
    : radius(radius), subdivision(subdivision)
{
    const float horizontalAngle = glm::radians(360.0f / 5); // Angle of each verticies pentagon
    const float verticalAngle = atanf(1.0f / 2); // Angle from center of the icosahedron to each vertices of pentagon
    float startAngle[] = {glm::radians(36.0f), glm::radians(72.0f)};
    int elevationMultiplier = 1;

    // Tip vertice
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

            startAngle[i] += horizontalAngle;
        }
        elevationMultiplier = -1;
    }

    indices = {
        0,  2,  3,
        0,  3,  4,
        0,  4,  5,
        0,  5,  6,
        0,  6,  2,

        1,  7,  8,
        1,  8,  9,
        1,  9, 10,
        1, 10, 11,
        1, 11,  7,


        6,  2, 11,
        11, 2,  7,
        2,  3,  7,
        7,  3,  8,
        3,  4,  8,
        8,  4,  9,
        4,  5,  9,
        9,  5, 10,
        5,  6, 10,
        10, 6, 11,

        //6,  2, 11,
        //11, 7, 2
    };

    // Indices
    /*
    0,  1,  2
    0,  2,  3
    0,  3,  4
    0,  4,  5
    0,  5,  1

    6,  7,  8
    6,  8,  9
    6,  9, 10
    6, 10, 11
    6, 11,  7



    0,  2,  3
    0,  3,  4
    0,  4,  5
    0,  5,  6
    0,  6,  2
    */
}

std::vector<float> icosphere::getVertices()
{
    return vertices;
}

std::vector<unsigned int> icosphere::getIndices()
{
    return indices;
}