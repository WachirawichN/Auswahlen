#include "geometry/icosphere.cuh"

#include <iostream>

__host__ void generateIcosahedron(float radius, std::vector<float>* vertices, std::vector<unsigned int>* indices)
{
    const float horizontalAngle = GLC::radians(360.0f / 5); // Angle of each verticies pentagon
    const float verticalAngle = atanf(1.0f / 2); // Angle from center of the icosahedron to each vertices of pentagon
    float startAngle[] = {GLC::radians(36.0f), GLC::radians(72.0f)};
    int elevationMultiplier = 1;

    // Tip / Bottom vertiex
    *vertices = {
        0.0f,  radius, 0.0f,
        0.0f, -radius, 0.0f,
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

__host__ std::vector<GLC::vec3> getMiddleVertices(int indexA, int indexB, int indexC, std::vector<float> vertices)
{
    GLC::vec3 vertexA(vertices.at(indexA * 3), vertices.at(indexA * 3 + 1), vertices.at(indexA * 3 + 2));
    GLC::vec3 vertexB(vertices.at(indexB * 3), vertices.at(indexB * 3 + 1), vertices.at(indexB * 3 + 2));
    GLC::vec3 vertexC(vertices.at(indexC * 3), vertices.at(indexC * 3 + 1), vertices.at(indexC * 3 + 2));

    std::vector<GLC::vec3> middleVertices = {
        (vertexA + vertexB) * 0.5f,
        (vertexA + vertexC) * 0.5f,
        (vertexB + vertexC) * 0.5f,
    };

    return middleVertices;
}
__host__ std::vector<GLC::vec2> getMiddleTexture(int indexA, int indexB, int indexC, std::vector<float> vertices)
{
    // Unused
    GLC::vec2 textureA(vertices.at(indexA * 5 + 3), vertices.at(indexA * 5 + 4));
    GLC::vec2 textureB(vertices.at(indexB * 5 + 3), vertices.at(indexB * 5 + 4));
    GLC::vec2 textureC(vertices.at(indexC * 5 + 3), vertices.at(indexC * 5 + 4));

    std::vector<GLC::vec2> middleTextures = {
        (textureA + textureB) * 0.5f,
        (textureA + textureC) * 0.5f,
        (textureB + textureC) * 0.5f,
    };

    return middleTextures;
}
__host__ GLC::vec3 calculateNewCoordinate(GLC::vec3 vertices, float radius)
{
    GLC::vec3 normalizedVertice(GLC::normalize(vertices));
    normalizedVertice *= radius;
    return normalizedVertice;
}

__host__ void subdivideIcosahedron(std::vector<float>* vertices, std::vector<unsigned int>* indices, float radius)
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

        std::vector<GLC::vec3> middleVertices = getMiddleVertices(indexA, indexB, indexC, *vertices);

        // Create three new vertices at each middle point between old vertices
        // Order from left to right, top to bottom
        int startIndex = vertices->size() / 5;
        for (int j = 0; j < 3; j++)
        {
            GLC::vec3 newVertices = calculateNewCoordinate(middleVertices[j], radius);
            vertices->push_back(newVertices[0]);
            vertices->push_back(newVertices[1]);
            vertices->push_back(newVertices[2]);
        }
        // Update indices
        // First triangle
        indices->push_back(indexA); // Old vertex
        indices->push_back(vertices->size() / 3 - 1 - 2); // First new vertices
        indices->push_back(vertices->size() / 3 - 1 - 1); // Second new vertices

        // Second triangle
        indices->push_back(indexB); // Old vertex
        indices->push_back(vertices->size() / 3 - 1 - 2); // First new vertices
        indices->push_back(vertices->size() / 3 - 1); // Third new vertices

        // Third triangle
        indices->push_back(vertices->size() / 3 - 1 - 2); // First new vertices
        indices->push_back(vertices->size() / 3 - 1 - 1); // Second new vertices
        indices->push_back(vertices->size() / 3 - 1); // Third new vertices

        // Fourth triangle
        indices->push_back(indexC); // Old vertex
        indices->push_back(vertices->size() / 3 - 1- 1); // Second new vertices
        indices->push_back(vertices->size() / 3 - 1); // Third new vertices
    }
}

__host__ geometry::icosphere::icosphere(float radius, unsigned int subdivision)
    : radius(radius), subdivision(subdivision)
{
    generateIcosahedron(radius, &vertices, &indices);

    for (int i = 0; i < subdivision - 1; i++)
    {
        subdivideIcosahedron(&vertices, &indices, radius);
    }
}
__host__ __device__ float geometry::icosphere::getRadius() const
{
    return radius;
}