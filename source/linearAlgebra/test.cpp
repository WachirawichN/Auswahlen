#include <iostream>

#include "vector/vector.h"

int main()
{
    {
        vector::vec4 testVec1(1.0f, 2.0f, 3.0f, 4.0f);
        vector::vec4 testVec2(5.0f, 6.0f, 7.0f, 8.0f);

        testVec1 += testVec2;
        std::cout << testVec1[0] << ", " << testVec1[1] << ", " << testVec1[2] << ", " << testVec1[3] << std::endl << std::endl;

        testVec1 -= testVec2;
        std::cout << testVec1[0] << ", " << testVec1[1] << ", " << testVec1[2] << ", " << testVec1[3] << std::endl << std::endl;

        testVec1 *= 2.5f;
        std::cout << testVec1[0] << ", " << testVec1[1] << ", " << testVec1[2] << ", " << testVec1[3] << std::endl << std::endl;

        testVec1 /= 2.5f;
        std::cout << testVec1[0] << ", " << testVec1[1] << ", " << testVec1[2] << ", " << testVec1[3] << std::endl << std::endl;
    }
    return 0;
}