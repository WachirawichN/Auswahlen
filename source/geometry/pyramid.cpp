#include "pyramid.h"

geometry::pyramid::pyramid(float width, float height, float depth)
    : width(width), height(height), depth(depth)
{
    vertices = {
    //                   Vertices                 
          0,           (height / 2),            0,

        -(width / 2), -(height / 2), -(depth / 2),
         (width / 2), -(height / 2), -(depth / 2),
         (width / 2), -(height / 2),  (depth / 2),
        -(width / 2), -(height / 2),  (depth / 2),
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
