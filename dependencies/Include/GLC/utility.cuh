#pragma once

#include <cmath>
#include <type_traits>
#include <concepts>
#include <numbers>
#include <math_constants.h>

#include <GLC/matrix.cuh>
#include <GLC/vector.cuh>

namespace GLC
{
    /*------------------------------------------------------------
        Common math funtions
        Available for both matrix and vector.
    ------------------------------------------------------------*/

    /**
     * @brief Raised vector to power of exponent.
     * 
     * Raised every element inside vector or matrix to the power of the exponent.
     * 
     * @tparam T Any vector type (vec2, vec3, vec4).
     * @param vector Vector we want to raise all the values to the power of exponent.
     * @param exponent Exponent of the vector.
     * @return Raised vector.
     */
    template<vectorType T>
    __host__ __device__ T pow(const T& vector, float exponent)
    {
        T out;
        for (int i = 0; i < (int)(sizeof(T) / sizeof(float)); i++)
        {
            #ifdef __CUDA_ARCH__
                out[i] = powf(vector[i], exponent);
            #else
                out[i] = std::powf(vector[i], exponent);
            #endif
        }
        return out;
    }
    /**
     * @brief Raised matrix to power of exponent.
     * 
     * Raised every element inside vector or matrix to the power of the exponent.
     * 
     * @tparam T Any matrix type (mat2, mat3, mat4).
     * @param matrix Matrix we want to raise all the values to the power of exponent.
     * @param exponent Exponent of the matrix.
     * @return Raised matrix.
     */
    template<matrixType T>
    __host__ __device__ T pow(const T& matrix, float exponent)
    {
        T out;
        #ifdef __CUDA_ARCH__
            int size = (int)sqrtf(sizeof(T) / sizeof(float));
        #else
            int size = (int)std::sqrtf(sizeof(T) / sizeof(float));
        #endif
        for (int i = 0; i < size; i++)
        {
            out[i] = pow(matrix[i], exponent);
        }
        return out;
    }
    /**
     * @brief Turn degree into radians.
     * @param degree Degree we want to convert to radians.
     * @return Degree that has been turned into radians.
     */
    __host__ __device__ float radians(float degree);

    /*------------------------------------------------------------
        Graphic funtions
    ------------------------------------------------------------*/

    /**
     * @brief Generate perspective projection matrix.
     * @param fov Fov of the camera.
     * @param aspect Aspect ratio of the camera.
     * @param near Length before the object will be clipped with camera.
     * @param far Length before the object will be unrendered due to how far it is.
     * @return Perspective projection matrix.
     */
    __host__ mat4 perspective(float fov, float aspect, float near, float far);
    /**
     * @brief Generate orthographic projection matrix. (Untested)
     * @param left Left coordinate of the frustum(Usually 0.0f).
     * @param right Right coordinate of the frustum.
     * @param bottom Bottom coordinate of the frustum(Usually 0.0f).
     * @param top Top coordinate of the frustum.
     * @param near Length before the object will be clipped with camera.
     * @param far Length before the object will be unrendered due to how far it is.
     * @return Orthographic projection matrix.
     */
    __host__ mat4 ortho(float left, float right, float bottom, float top, float near, float far);
    /**
     * @brief Generate matrix of where camera look at.
     * @param position Position of camera.
     * @param target Target that where camera look at.
     * @param up Up vector.
     * @return matrix where camera look at.
     */
    __host__ mat4 lookAt(const vec3& position, const vec3& target, const vec3& up);

    /*------------------------------------------------------------
        Model funtions
    ------------------------------------------------------------*/

    /**
     * @brief Generate translation matrix.
     * @param offset Vector of how much translation we want.
     * @return Translation matrix.
     */
    __host__ __device__ mat4 translate(const vec3& offset);
    /**
     * @overload
     * @brief Apply translation into the matrix.
     * @param offset Vector of how much translation we want.
     * @param matrix Matrix to apply translation to.
     * @return Translated version of the input matrix.
     */
    __host__ __device__ mat4 translate(const vec3& offset, const mat4& matrix);
    /**
     * @brief Generate rotation matrix.
     * @param angle How much angle we want to rotate.
     * @param axis Vector of axis we want to rotate.
     * @return Rotation matrix.
     */
    __host__ __device__ mat4 rotate(float angle, const vec3& axis);
    /**
     * @overload
     * @brief Apply rotation to the input matrix.
     * @param angle How much angle we want to rotate.
     * @param axis Vector of axis we want to rotate.
     * @param matrix Matrix that we want to apply rotation to.
     * @return Rotated version of the input matrix.
     */
    __host__ __device__ mat4 rotate(float angle, const vec3& axis, const mat4& matrix);
    /**
     * @brief Generate scaling matrix.
     * @param factor How much we want to scale on each axis.
     * @return Scaling matrix.
     */
    __host__ __device__ mat4 scale(const vec3& factor);
    /**
     * @overload
     * @brief Scale the input matrix.
     * @param factor How much we want to scale on each axis.
     * @param matrix Matrix we want to apply scaling to. (Scaling will be multiply on the old one)
     * @return Scaled version of the input matrix.
     */
    __host__ __device__ mat4 scale(const vec3& factor, const mat4& matrix);

    /*------------------------------------------------------------
        Other funtions
        Some functions is available for both matrix and vector.
    ------------------------------------------------------------*/

    /**
     * @brief Unpack vector into array of float. Similar to value_ptr of GLM.
     * @tparam T Any vector type (vec2, vec3, vec4).
     * @param vector Vector we want to unpack.
     * @return Pointer to array of float containing every values of the input vector.
     */
    template<vectorType T>
    __host__ __device__ float* unpack(const T& vector)
    {
        static float out[sizeof(T) / sizeof(float)];
        for (int i = 0; i < sizeof(T) / sizeof(float); i++)
        {
            out[i] = vector[i];
        }
        return out;
    }
    /**
     * @overload
     * @brief Unpack matrix into array of float. Similar to value_ptr of GLM.
     * @tparam T Any matrix type (mat2, mat3, mat4).
     * @param matrix Matrix we want to unpack.
     * @return Pointer to array of float containing every values of the input matrix.
     */
    template<matrixType T>
    __host__ __device__ float* unpack(const T& matrix)
    {
        static float out[sizeof(T) / sizeof(float)];
        #ifdef __CUDA_ARCH__
            int size = (int)sqrtf(sizeof(T) / sizeof(float));
        #else
            int size = (int)std::sqrtf(sizeof(T) / sizeof(float));
        #endif
        for (int column = 0; column < size; column++)
        {
            for (int row = 0; row < size; row++)
            {
                out[column * size + row] = matrix[column][row];
            }
        }
        return out;
    }
    /**
     * @brief Get thread ID of current CUDA thread.
     * @return Thread ID.
     */
    __device__ int threadID();
}