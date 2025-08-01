#include <GLC/vector.cuh>

namespace GLC
{
    __host__ __device__ float& vec4::operator[](unsigned int index)
    {
        return *(&x + index);
    }
    __host__ __device__ float vec4::operator[](unsigned int index) const
    {
        return *(&x + index);
    }

    __host__ __device__ vec4 vec4::operator+(float scalar) const
    {
        return *this + vec4(scalar);
    }
    __host__ __device__ vec4& vec4::operator+=(float scalar)
    {
        return *this += vec4(scalar);
    }
    __host__ __device__ vec4 vec4::operator+(const vec4& vector) const
    {
        vec4 out;
        for (int i = 0; i < 4; i++)
        {
            out[i] = *(&x + i) + vector[i];
        }
        return out;
    }
    __host__ __device__ vec4& vec4::operator+=(const vec4& vector)
    {
        for (int i = 0; i < 4; i++)
        {
            *(&x + i) += vector[i];
        }
        return *this;
    }

    __host__ __device__ vec4 vec4::operator-(float scalar) const
    {
        return *this - vec4(scalar);
    }
    __host__ __device__ vec4& vec4::operator-=(float scalar)
    {
        return *this -= vec4(scalar);
    }
    __host__ __device__ vec4 vec4::operator-(const vec4& vector) const
    {
        vec4 out;
        for (int i = 0; i < 4; i++)
        {
            out[i] = *(&x + i) - vector[i];
        }
        return out;
    }
    __host__ __device__ vec4& vec4::operator-=(const vec4& vector)
    {
        for (int i = 0; i < 4; i++)
        {
            *(&x + i) -= vector[i];
        }
        return *this;
    }

    __host__ __device__ vec4 vec4::operator*(float scalar) const
    {
        vec4 out;
        for (int i = 0; i < 4; i++)
        {
            out[i] = *(&x + i) * scalar;
        }
        return out;
    }
    __host__ __device__ vec4& vec4::operator*=(float scalar)
    {
        for (int i = 0; i < 4; i++)
        {
            *(&x + i) *= scalar;
        }
        return *this;
    }

    __host__ __device__ vec4 vec4::operator/(float scalar) const
    {
        vec4 out;
        for (int i = 0; i < 4; i++)
        {
            out[i] = *(&x + i) / scalar;
        }
        return out;
    }
    __host__ __device__ vec4& vec4::operator/=(float scalar)
    {
        for (int i = 0; i < 4; i++)
        {
            *(&x + i) /= scalar;
        }
        return *this;
    }

    __host__ std::ostream& operator << (std::ostream& os, const vec4& vector)
    {
        return os << "[" << vector[0] << ", " << vector[1] << ", " << vector[2] << ", " << vector[3] << "]";
    }
}