#include "geometry/cube.cuh"

__host__ geometry::cube::cube(float width, float height, float depth)
    : width(width), height(height), depth(depth)
{
    vertices = {
    //                   Vertices
        -(width / 2), -(height / 2), -(depth / 2),
         (width / 2), -(height / 2), -(depth / 2),
         (width / 2),  (height / 2), -(depth / 2),
        -(width / 2),  (height / 2), -(depth / 2),
        
        -(width / 2), -(height / 2),  (depth / 2),
         (width / 2), -(height / 2),  (depth / 2),
         (width / 2),  (height / 2),  (depth / 2),
        -(width / 2),  (height / 2),  (depth / 2),
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

__host__ __device__ float geometry::cube::getWidth() const
{
    return width;
}

__host__ __device__ float geometry::cube::getDepth() const
{
    return depth;
}

__host__ __device__ float geometry::cube::getHeight() const
{
    return height;
}