#include "icosphere.h"

#include <iostream>

void generateIcosahedron(float radius, std::vector<float>* vertices, std::vector<unsigned int>* indices)
{
    const float horizontalAngle = glm::radians(360.0f / 5); // Angle of each verticies pentagon
    const float verticalAngle = atanf(1.0f / 2); // Angle from center of the icosahedron to each vertices of pentagon
    float startAngle[] = {glm::radians(36.0f), glm::radians(72.0f)};
    int elevationMultiplier = 1;

    // Tip / Bottom vertiex
    *vertices = {
        0.0f,  radius, 0.0f,        0.0f, 0.0f,
        0.0f, -radius, 0.0f,        1.0f, 1.0f,
    };

    // Generating Icosahedron
    // Order from left to right, top to bottom
    for (int i = 0; i < 2; i++) // Row
    {
        for (int j = 0; j < 5; j++) // Column
        {
            // X
            vertices->push_back(radius * cosf(verticalAngle) * cosf(startAngle[i]));
            // Y
            vertices->push_back(radius * sinf(verticalAngle) * elevationMultiplier);
            // Z
            vertices->push_back(radius * cosf(verticalAngle) * sinf(startAngle[i]));
            // Texture coordinates
            vertices->push_back(1.0f / 5 * (j + 1)); // Horizontal texture
            vertices->push_back(1.0f / 3 * (i + 1)); // Vertical texture

            // Indices
            // Connect to tip / bottom vertice
            indices->push_back(i); // Tip / Bottom vertex
            indices->push_back((2 + (i * 5)) + j); // Current vertex
            indices->push_back((2 + (i * 5)) + ((j + 1) % 5)); // Next vertex

            // Connect middle section
            indices->push_back((2 + (i * 5)) + j); // Current vertex
            indices->push_back((2 + (i * 5)) + ((j + 1) % 5)); // Next vertex
            indices->push_back(indices->at(indices->size() -1 + (!i * - 1)) + (5 * (i * -2 + 1))); // Above / Below vertex
            
            startAngle[i] += horizontalAngle;
        }
        elevationMultiplier = -1;
    }
}

std::vector<glm::vec3> getMiddleVertices(int indexA, int indexB, int indexC, std::vector<float>* vertices)
{
    glm::vec3 vertexA(vertices->at(indexA * 5), vertices->at(indexA * 5 + 1), vertices->at(indexA * 5 + 2));
    glm::vec3 vertexB(vertices->at(indexB * 5), vertices->at(indexB * 5 + 1), vertices->at(indexB * 5 + 2));
    glm::vec3 vertexC(vertices->at(indexC * 5), vertices->at(indexC * 5 + 1), vertices->at(indexC * 5 + 2));

    std::vector<glm::vec3> middleVertices = {
        (vertexA + vertexB) * 0.5f,
        (vertexA + vertexC) * 0.5f,
        (vertexB + vertexC) * 0.5f,
    };

    return middleVertices;
}

std::vector<glm::vec2> getMiddleTexture(int indexA, int indexB, int indexC, std::vector<float>* vertices)
{
    glm::vec2 textureA(vertices->at(indexA * 5 + 3), vertices->at(indexA * 5 + 4));
    glm::vec2 textureB(vertices->at(indexB * 5 + 3), vertices->at(indexB * 5 + 4));
    glm::vec2 textureC(vertices->at(indexC * 5 + 3), vertices->at(indexC * 5 + 4));

    std::vector<glm::vec2> middleTextures = {
        (textureA + textureB) * 0.5f,
        (textureA + textureC) * 0.5f,
        (textureB + textureC) * 0.5f,
    };

    return middleTextures;
}

void subdivideIcosahedron(int subdivideAmount, std::vector<float>* vertices, std::vector<unsigned int>* indices)
{

}

icosphere::icosphere(float radius, int subdivision)
    : radius(radius), subdivision(subdivision)
{
    generateIcosahedron(radius, &vertices, &indices);

    // Amount of row
    /*
    int totalIteration = 2;
    for (int i = 0; i < subdivision - 1; i++)
    {
        totalIteration += (pow(2, i) * 3);
    }
    */

    // Adding subdivision into icosahedron to turn it into icosphere
    // Method using may cause more memory to use more than it should because some new vertices maybe duplicate
    // TLDR Extreme unreadable / unoptimized
    for (int i = 0; i < subdivision - 1; i++)
    {
        int currentIndicesSize = indices.size();
        for (int j = 0; j < currentIndicesSize; j += 3) // Loop through each triangle
        {
            // Calculate distance between each vertices
            int indexA = indices.at(j);
            int indexB = indices.at(j + 1);
            int indexC = indices.at(j + 2);

            std::vector<glm::vec3> middleVertices = getMiddleVertices(indexA, indexB, indexC, &vertices);
            std::vector<glm::vec2> middleTextures = getMiddleTexture(indexA, indexB, indexC, &vertices);

            // Create three new vertices at each middle point between old vertices
            // Order from left to right, top to bottom
            int startIndex = vertices.size() / 5;
            for (int k = 0; k < 3; k++)
            {
                float* vertex = glm::value_ptr(middleVertices[k]);
                float* texture = glm::value_ptr(middleTextures[k]);

                vertices.push_back(vertex[0]);
                vertices.push_back(vertex[1]);
                vertices.push_back(vertex[2]);

                vertices.push_back(texture[0]);
                vertices.push_back(texture[1]);
            }

            // Update indices
            indices.erase(indices.begin() + (j - 1), indices.begin() + (j + 1)); // Remove old triangle vertices
            for (int k = 0; k < 4; k++)
            {
                /* code */
            }
            
            indices.push_back(indexA);
            indices.push_back(startIndex);
            startIndex += 1;
            indices.push_back(startIndex);
            startIndex += 1;
        }
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