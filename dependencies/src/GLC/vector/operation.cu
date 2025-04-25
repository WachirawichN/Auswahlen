#include "../../include/vector.cuh"

namespace GLC
{
    __host__ __device__ vec2 cross(const vec2& a, const vec2& b)
    {
        // [Ax]   [Bx]   [Ay⋅Bx - Ay⋅Bx]
        // [Ay] x [By] = [Ax⋅By − Ax⋅By]
        // Return 0 for vec2
        return vec2();
    }
    __host__ __device__ vec3 cross(const vec3& a, const vec3& b)
    {
        // [Ax]   [Bx]   [Ay⋅Bz - Az⋅By]
        // [Ay] x [By] = [Az⋅Bx − Ax⋅Bz]
        // [Az]   [Bz]   [Ax⋅By − Ay⋅Bx]
        return vec3(
            a[1] * b[2] - a[2] * b[1],
            a[2] * b[0] - a[0] * b[2],
            a[0] * b[1] - a[1] * b[0]
        );
    }
    __host__ __device__ vec4 cross(const vec4& a, const vec4& b)
    {
        // [Ax]   [Bx]   [Ay⋅Bz - Aω⋅Bz]
        // [Ay] x [By] = [Az⋅Bω − Ax⋅Bω]
        // [Az]   [Bz]   [Aω⋅Bx − Ay⋅Bx]
        // [Aω]   [Bω]   [Ax⋅By - Az⋅By]
        return vec4(
            b[2] * (a[1] - a[3]),
            b[3] * (a[2] - a[0]),
            b[0] * (a[3] - a[1]),
            b[1] * (a[0] - a[2])
        );
    }

    __host__ __device__ float length(const vec2& vector)
    {
        #ifdef __CUDA_ARCH__
            return hypotf(vector[0], vector[1]);
        #else
            return std::sqrtf(std::powf(vector[0], 2.0f) + std::powf(vector[1], 2.0f));
        #endif
    }
    __host__ __device__ float length(const vec3& vector)
    {
        #ifdef __CUDA_ARCH__
            return norm3df(vector[0], vector[1], vector[2]);
        #else
            return std::sqrtf(std::powf(vector[0], 2.0f) + std::powf(vector[1], 2.0f) + std::powf(vector[2], 2.0f));
        #endif
    }
    __host__ __device__ float length(const vec4& vector)
    {
        #ifdef __CUDA_ARCH__
            return norm4df(vector[0], vector[1], vector[2], vector[3]);
        #else
            return std::sqrtf(std::powf(vector[0], 2.0f) + std::powf(vector[1], 2.0f) + std::powf(vector[2], 2.0f) + std::powf(vector[3], 2.0f));
        #endif
    }
}