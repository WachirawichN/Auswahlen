#pragma once

#include <iostream>
#include <string>
#include <type_traits>
#include <concepts>
#include <cmath>

#include "vector.cuh"

// This will be weird due to how weird GLSL's matrix structure is. (Column major)
// 3x3 Matrix example bruh
// ┌ ┌   ┐ ┌   ┐ ┌   ┐ ┐
// | | a | | d | | g | |
// | | b | | e | | h | |
// | | c | | f | | i | |
// └ └   ┘ └   ┘ └   ┘ ┘
// Printing out matrix will result in slighty different formation than this tho, console doesn't support printing those special char.

namespace GLC
{
    class mat2
    {
        private:
            vec2 value[2];
        public:
            __host__ __device__ mat2();
            __host__ __device__ mat2(float v0);
            __host__ __device__ mat2(const vec2& v0, const vec2& v1);

            __host__ __device__ vec2& operator [] (unsigned int index);
            __host__ __device__ const vec2& operator [] (unsigned int index) const;
            __host__ __device__ mat2& operator = (const mat2& matrix);

            __host__ __device__ mat2 operator + (const mat2& matrix) const;
            __host__ __device__ mat2& operator += (const mat2& matrix);

            __host__ __device__ mat2 operator - (const mat2& matrix) const;
            __host__ __device__ mat2& operator -= (const mat2& matrix);

            __host__ __device__ mat2 operator * (float scalar) const;
            __host__ __device__ mat2& operator *= (float scalar);
            __host__ __device__ mat2 operator * (const mat2& matrix) const;
            __host__ __device__ mat2& operator *= (const mat2& matrix);
            __host__ __device__ vec2 operator * (const vec2& vector) const;

            __host__ __device__ mat2 operator / (float scalar) const;
            __host__ __device__ mat2& operator /= (float scalar);

            __host__ friend std::ostream& operator << (std::ostream& os, const mat2& matrix);
    };
    class mat3
    {
        private:
            vec3 value[3];
        public:
            __host__ __device__ mat3();
            __host__ __device__ mat3(float v0);
            __host__ __device__ mat3(const vec3& v0, const vec3& v1, const vec3& v2);

            __host__ __device__ vec3& operator [] (unsigned int index);
            __host__ __device__ const vec3& operator [] (unsigned int index) const;
            __host__ __device__ mat3& operator = (const mat3& matrix);

            __host__ __device__ mat3 operator + (const mat3& matrix) const;
            __host__ __device__ mat3& operator += (const mat3& matrix);

            __host__ __device__ mat3 operator - (const mat3& matrix) const;
            __host__ __device__ mat3& operator -= (const mat3& matrix);

            __host__ __device__ mat3 operator * (float scalar) const;
            __host__ __device__ mat3& operator *= (float scalar);
            __host__ __device__ mat3 operator * (const mat3& matrix) const;
            __host__ __device__ mat3& operator *= (const mat3& matrix);
            __host__ __device__ vec3 operator * (const vec3& vector) const;

            __host__ __device__ mat3 operator / (float scalar) const;
            __host__ __device__ mat3& operator /= (float scalar);

            __host__ friend std::ostream& operator << (std::ostream& os, const mat3& matrix);
    };
    class mat4
    {
        private:
            vec4 value[4];
        public:
            __host__ __device__ mat4();
            __host__ __device__ mat4(float v0);
            __host__ __device__ mat4(const vec4& v0, const vec4& v1, const vec4& v2, const vec4& v3);

            __host__ __device__ vec4& operator [] (unsigned int index);
            __host__ __device__ const vec4& operator [] (unsigned int index) const;
            __host__ __device__ mat4& operator = (const mat4& matrix);

            __host__ __device__ mat4 operator + (const mat4& matrix) const;
            __host__ __device__ mat4& operator += (const mat4& matrix);

            __host__ __device__ mat4 operator - (const mat4& matrix) const;
            __host__ __device__ mat4& operator -= (const mat4& matrix);

            __host__ __device__ mat4 operator * (float scalar) const;
            __host__ __device__ mat4& operator *= (float scalar);
            __host__ __device__ mat4 operator * (const mat4& matrix) const;
            __host__ __device__ mat4& operator *= (const mat4& matrix);
            __host__ __device__ vec4 operator * (const vec4& vector) const;

            __host__ __device__ mat4 operator / (float scalar) const;
            __host__ __device__ mat4& operator /= (float scalar);

            __host__ friend std::ostream& operator << (std::ostream& os, const mat4& matrix);
    };

    template<typename T>
    concept matrixType = std::same_as<T, GLC::mat2> || std::same_as<T, GLC::mat3> || std::same_as<T, GLC::mat4>;

    /*------------------------------------------------------------
        Matrix exclusive functions
    ------------------------------------------------------------*/

    /**
     * @brief Transpose the input matrix.
     * @tparam T Any matrix type (mat2, mat3, mat4).
     * @param matrix Matrix we want to transpose.
     * @return Transposed version of the input matrix.
     */
    template<matrixType T>
    __host__ __device__ T transpose(const T& matrix)
    {
        T out;
        #ifdef __CUDA_ARCH__
            int size = (int)sqrtf(sizeof(T) / sizeof(float));
        #else
            int size = (int)std::sqrtf(sizeof(T) / sizeof(float));
        #endif
        for (int row = 0; row < size; row++)
        {
            for (int column = 0; column < size; column++)
            {
                out[row][column] = matrix[column][row];
            }
        }
        return out;
    }
}