#pragma once

#include <iostream>
#include <cmath>
#include <stdio.h>

namespace GLC
{
    class vec2
    {
        public:
            float x, y;

            __host__ __device__ vec2() : x(0.0f), y(0.0f) {}
            __host__ __device__ vec2(float v) : x(v), y(v) {}
            __host__ __device__ vec2(float v0, float v1) : x(v0), y(v1) {}

            __host__ __device__ float& operator [] (unsigned int index);
            __host__ __device__ float operator [] (unsigned int index) const;

            __host__ __device__ vec2 operator + (float scalar) const;
            __host__ __device__ vec2& operator += (float scalar);
            __host__ __device__ vec2 operator + (const vec2& vector) const;
            __host__ __device__ vec2& operator += (const vec2& vector);

            __host__ __device__ vec2 operator - (float scalar) const;
            __host__ __device__ vec2& operator -= (float scalar);
            __host__ __device__ vec2 operator - (const vec2& vector) const;
            __host__ __device__ vec2& operator -= (const vec2& vector);

            __host__ __device__ vec2 operator * (float scalar) const;
            __host__ __device__ vec2& operator *= (float scalar);

            __host__ __device__ vec2 operator / (float scalar) const;
            __host__ __device__ vec2& operator /= (float scalar);

            __host__ friend std::ostream& operator << (std::ostream& os, const vec2& vector);
    };
    class vec3
    {
        public:
            float x, y, z;

            __host__ __device__ vec3() : x(0.0f), y(0.0f), z(0.0f) {}
            __host__ __device__ vec3(float v) : x(v), y(v), z(v) {}
            __host__ __device__ vec3(float v0, float v1, float v2) : x(v0), y(v1), z(v2) {}

            __host__ __device__ float& operator [] (unsigned int index);
            __host__ __device__ float operator [] (unsigned int index) const;

            __host__ __device__ vec3 operator + (float scalar) const;
            __host__ __device__ vec3& operator += (float scalar);
            __host__ __device__ vec3 operator + (const vec3& vector) const;
            __host__ __device__ vec3& operator += (const vec3& vector);

            __host__ __device__ vec3 operator - (float scalar) const;
            __host__ __device__ vec3& operator -= (float scalar);
            __host__ __device__ vec3 operator - (const vec3& vector) const;
            __host__ __device__ vec3& operator -= (const vec3& vector);

            __host__ __device__ vec3 operator * (float scalar) const;
            __host__ __device__ vec3& operator *= (float scalar);

            __host__ __device__ vec3 operator / (float scalar) const;
            __host__ __device__ vec3& operator /= (float scalar);
            
            __host__ friend std::ostream& operator << (std::ostream& os, const vec3& vector);
    };
    class vec4
    {
        public:
            float x, y, z, w;

            __host__ __device__ vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
            __host__ __device__ vec4(float v) : x(v), y(v), z(v), w(v) {}
            __host__ __device__ vec4(float v0, float v1, float v2, float v3) : x(v0), y(v1), z(v2), w(v3) {}

            __host__ __device__ float& operator [] (unsigned int index);
            __host__ __device__ float operator [] (unsigned int index) const;

            __host__ __device__ vec4 operator + (float scalar) const;
            __host__ __device__ vec4& operator += (float scalar);
            __host__ __device__ vec4 operator + (const vec4& vector) const;
            __host__ __device__ vec4& operator += (const vec4& vector);

            __host__ __device__ vec4 operator - (float scalar) const;
            __host__ __device__ vec4& operator -= (float scalar);
            __host__ __device__ vec4 operator - (const vec4& vector) const;
            __host__ __device__ vec4& operator -= (const vec4& vector);

            __host__ __device__ vec4 operator * (float scalar) const;
            __host__ __device__ vec4& operator *= (float scalar);

            __host__ __device__ vec4 operator / (float scalar) const;
            __host__ __device__ vec4& operator /= (float scalar);

            __host__ friend std::ostream& operator << (std::ostream& os, const vec4& vector);
    };

    template<typename T>
    concept vectorType = std::same_as<T, GLC::vec2> || std::same_as<T, GLC::vec3> || std::same_as<T, GLC::vec4>;

    /*------------------------------------------------------------
        Vector exclusive functions
    ------------------------------------------------------------*/

    /**
     * @brief Perform cross product on two input vector.
     * @param a Vector A.
     * @param b Vector B.
     * @return Vector which is result of performing cross product on two input vectors.
     */
    __host__ __device__ vec2 cross(const vec2& a, const vec2& b);
    /**
     * @brief Perform cross product on two input vector.
     * @param a Vector A.
     * @param b Vector B.
     * @return Vector which is result of performing cross product on two input vectors.
     */
    __host__ __device__ vec3 cross(const vec3& a, const vec3& b);
    /**
     * @brief Perform cross product on two input vector.
     * @param a Vector A.
     * @param b Vector B.
     * @return Vector which is result of performing cross product on two input vectors.
     */
    __host__ __device__ vec4 cross(const vec4& a, const vec4& b);

    /**
     * @brief Perform dot product on two input vector.
     * @tparam T Any vector type (vec2, vec3, vec4).
     * @param a Vector A.
     * @param b Vector B.
     * @return Result of dot product.
     */
    template<vectorType T>
    __host__ __device__ float dot(const T& a, const T& b)
    {
        float sum = 0.0f;
        for (int i = 0; i < (int)(sizeof(T) / sizeof(float)); i++)
        {
            sum += a[i] * b[i];
        }
        return sum;
    }

    /**
     * @brief Calculate the length of input vector.
     * @param a Vector we want to find the length of.
     * @return Length of the vector.
     */
    __host__ __device__ float length(const vec2& vector);
    /**
     * @brief Calculate the length of input vector.
     * @param a Vector we want to find the length of.
     * @return Length of the vector.
     */
    __host__ __device__ float length(const vec3& vector);
    /**
     * @brief Calculate the length of input vector.
     * @param a Vector we want to find the length of.
     * @return Length of the vector.
     */
    __host__ __device__ float length(const vec4& vector);

    /**
     * @brief Normalize the input vector.
     * @tparam T Any vector type (vec2, vec3, vec4).
     * @param vector Vector we want to normalize.
     * @return Normalized version of the input vector.
     */
    template<vectorType T>
    __host__ __device__ T normalize(const T& vector)
    {
        return vector / length(vector);
    }
}