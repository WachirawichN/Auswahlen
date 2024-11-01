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
            // Connect to tip / bottom vertex
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

std::vector<glm::vec3> getMiddleVertices(int indexA, int indexB, int indexC, std::vector<float> vertices)
{
    glm::vec3 vertexA(vertices.at(indexA * 5), vertices.at(indexA * 5 + 1), vertices.at(indexA * 5 + 2));
    glm::vec3 vertexB(vertices.at(indexB * 5), vertices.at(indexB * 5 + 1), vertices.at(indexB * 5 + 2));
    glm::vec3 vertexC(vertices.at(indexC * 5), vertices.at(indexC * 5 + 1), vertices.at(indexC * 5 + 2));

    std::vector<glm::vec3> middleVertices = {
        (vertexA + vertexB) * 0.5f,
        (vertexA + vertexC) * 0.5f,
        (vertexB + vertexC) * 0.5f,
    };

    return middleVertices;
}

std::vector<glm::vec2> getMiddleTexture(int indexA, int indexB, int indexC, std::vector<float> vertices)
{
    glm::vec2 textureA(vertices.at(indexA * 5 + 3), vertices.at(indexA * 5 + 4));
    glm::vec2 textureB(vertices.at(indexB * 5 + 3), vertices.at(indexB * 5 + 4));
    glm::vec2 textureC(vertices.at(indexC * 5 + 3), vertices.at(indexC * 5 + 4));

    std::vector<glm::vec2> middleTextures = {
        (textureA + textureB) * 0.5f,
        (textureA + textureC) * 0.5f,
        (textureB + textureC) * 0.5f,
    };

    return middleTextures;
}

glm::vec3 calculateNewCoordinate(glm::vec3 vertices, float radius)
{
    glm::vec3 normalizedVertice(glm::normalize(vertices));
    normalizedVertice *= radius;
    return normalizedVertice;
}

void subdivideIcosahedron(std::vector<float>* vertices, std::vector<unsigned int>* indices, float radius)
{
    // Adding subdivision into icosahedron to turn it into icosphere
    // Method using may cause more memory to use more than it should because some new vertices maybe duplicate
    // TLDR Extreme unreadable / unoptimized
    int originalIndicesSize = indices->size();
    for (int i = 0; i < originalIndicesSize; i += 3) // Loop through each triangle
    {

        // Calculate distance between each vertices
        int indexA = indices->at(0);
        int indexB = indices->at(1);
        int indexC = indices->at(2);

        // Remove old first three indices
        indices->erase(indices->begin(), indices->begin() + 3);

        std::vector<glm::vec3> middleVertices = getMiddleVertices(indexA, indexB, indexC, *vertices);
        std::vector<glm::vec2> middleTextures = getMiddleTexture(indexA, indexB, indexC, *vertices);

        // Create three new vertices at each middle point between old vertices
        // Order from left to right, top to bottom
        int startIndex = vertices->size() / 5;
        for (int j = 0; j < 3; j++)
        {
            glm::vec3 newVertices = calculateNewCoordinate(middleVertices[j], radius);
            vertices->push_back(newVertices.x);
            vertices->push_back(newVertices.y);
            vertices->push_back(newVertices.z);

            vertices->push_back(middleTextures[j].x);
            vertices->push_back(middleTextures[j].y);
        }
        // Update indices
        // First triangle
        indices->push_back(indexA); // Old vertex
        indices->push_back(vertices->size() / 5 - 1 - 2); // First new vertices
        indices->push_back(vertices->size() / 5 - 1 - 1); // Second new vertices

        // Second triangle
        indices->push_back(indexB); // Old vertex
        indices->push_back(vertices->size() / 5 - 1 - 2); // First new vertices
        indices->push_back(vertices->size() / 5 - 1); // Third new vertices

        // Third triangle
        indices->push_back(vertices->size() / 5 - 1 - 2); // First new vertices
        indices->push_back(vertices->size() / 5 - 1 - 1); // Second new vertices
        indices->push_back(vertices->size() / 5 - 1); // Third new vertices

        // Fourth triangle
        indices->push_back(indexC); // Old vertex
        indices->push_back(vertices->size() / 5 - 1- 1); // Second new vertices
        indices->push_back(vertices->size() / 5 - 1); // Third new vertices
    }
}

geometry::icosphere::icosphere(float radius, int subdivision)
    : radius(radius), subdivision(subdivision)
{
    generateIcosahedron(radius, &vertices, &indices);

    for (int i = 0; i < subdivision - 1; i++)
    {
        subdivideIcosahedron(&vertices, &indices, radius);
    }
}

std::vector<float> geometry::icosphere::getVertices()
{
    return vertices;
}

std::vector<unsigned int> geometry::icosphere::getIndices()
{
    return indices;
}