#include "../../include/vector.cuh"

namespace GLC
{
    __host__ __device__ vec2::vec2()
    {
        for (int i = 0; i < 2; i++)
        {
            value[i] = 0.0f;
        }
    }
    __host__ __device__ vec2::vec2(float v0)
    {
        for (int i = 0; i < 2; i++)
        {
            value[i] = v0;
        }
    }
    __host__ __device__ vec2::vec2(float v0, float v1)
    {
        value[0] = v0;
        value[1] = v1;
    }
    __host__ __device__ vec2::vec2(const vec2& vector)
    {
        for (int i = 0; i < 2; i++)
        {
            value[i] = vector[i];
        }
    }

    __host__ __device__ float& vec2::operator[](unsigned int index)
    {
        return value[index];
    }
    __host__ __device__ float vec2::operator[](unsigned int index) const
    {
        return value[index];
    }
    __host__ __device__ vec2& vec2::operator=(const vec2& vector)
    {
        if (this != &vector)
        {
            for (int i = 0; i < 2; i++)
            {
                value[i] = vector[i];
            }
        }
        return *this;
    }

    __host__ __device__ vec2 vec2::operator+(float scalar) const
    {
        return *this + vec2(scalar);
    }
    __host__ __device__ vec2& vec2::operator+=(float scalar)
    {
        return *this += vec2(scalar);
    }
    __host__ __device__ vec2 vec2::operator+(const vec2& vector) const
    {
        vec2 out;
        for (int i = 0; i < 2; i++)
        {
            out[i] = value[i] + vector[i];
        }
        return out;
    }
    __host__ __device__ vec2& vec2::operator+=(const vec2& vector)
    {
        for (int i = 0; i < 2; i++)
        {
            value[i] += vector[i];
        }
        return *this;
    }

    __host__ __device__ vec2 vec2::operator-(float scalar) const
    {
        return *this - vec2(scalar);
    }
    __host__ __device__ vec2& vec2::operator-=(float scalar)
    {
        return *this -= vec2(scalar);
    }
    __host__ __device__ vec2 vec2::operator-(const vec2& vector) const
    {
        vec2 out;
        for (int i = 0; i < 2; i++)
        {
            out[i] = value[i] - vector[i];
        }
        return out;
    }
    __host__ __device__ vec2& vec2::operator-=(const vec2& vector)
    {
        for (int i = 0; i < 2; i++)
        {
            value[i] -= vector[i];
        }
        return *this;
    }

    __host__ __device__ vec2 vec2::operator*(float scalar) const
    {
        vec2 out;
        for (int i = 0; i < 2; i++)
        {
            out[i] = value[i] * scalar;
        }
        return out;
    }
    __host__ __device__ vec2& vec2::operator*=(float scalar)
    {
        for (int i = 0; i < 2; i++)
        {
            value[i] *= scalar;
        }
        return *this;
    }

    __host__ __device__ vec2 vec2::operator/(float scalar) const
    {
        vec2 out;
        for (int i = 0; i < 2; i++)
        {
            out[i] = value[i] / scalar;
        }
        return out;
    }
    __host__ __device__ vec2& vec2::operator/=(float scalar)
    {
        for (int i = 0; i < 2; i++)
        {
            value[i] /= scalar;
        }
        return *this;
    }

    __host__ std::ostream& operator << (std::ostream& os, const vec2& vector)
    {
        return os << "[" << vector[0] << ", " << vector[1] << "]";
    }
}