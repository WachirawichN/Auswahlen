#include <GLC/vector.cuh>

namespace GLC
{
    __host__ __device__ float& vec2::operator[](unsigned int index)
    {
        return *(&x + index);
    }
    __host__ __device__ float vec2::operator[](unsigned int index) const
    {
        return *(&x + index);
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
            out[i] = *(&x + i) + vector[i];
        }
        return out;
    }
    __host__ __device__ vec2& vec2::operator+=(const vec2& vector)
    {
        for (int i = 0; i < 2; i++)
        {
            *(&x + i) += vector[i];
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
            out[i] = *(&x + i) - vector[i];
        }
        return out;
    }
    __host__ __device__ vec2& vec2::operator-=(const vec2& vector)
    {
        for (int i = 0; i < 2; i++)
        {
            *(&x + i) -= vector[i];
        }
        return *this;
    }

    __host__ __device__ vec2 vec2::operator*(float scalar) const
    {
        vec2 out;
        for (int i = 0; i < 2; i++)
        {
            out[i] = *(&x + i) * scalar;
        }
        return out;
    }
    __host__ __device__ vec2& vec2::operator*=(float scalar)
    {
        for (int i = 0; i < 2; i++)
        {
            *(&x + i) *= scalar;
        }
        return *this;
    }

    __host__ __device__ vec2 vec2::operator/(float scalar) const
    {
        vec2 out;
        for (int i = 0; i < 2; i++)
        {
            out[i] = *(&x + i) / scalar;
        }
        return out;
    }
    __host__ __device__ vec2& vec2::operator/=(float scalar)
    {
        for (int i = 0; i < 2; i++)
        {
            *(&x + i) /= scalar;
        }
        return *this;
    }

    __host__ std::ostream& operator << (std::ostream& os, const vec2& vector)
    {
        return os << "[" << vector[0] << ", " << vector[1] << "]";
    }
}