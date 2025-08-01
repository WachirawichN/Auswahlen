#include <GLC/vector.cuh>

namespace GLC
{
    __host__ __device__ float& vec3::operator[](unsigned int index)
    {
        return *(&x + index);
    }
    __host__ __device__ float vec3::operator[](unsigned int index) const
    {
        return *(&x + index);
    }

    __host__ __device__ vec3 vec3::operator+(float scalar) const
    {
        return *this + vec3(scalar);
    }
    __host__ __device__ vec3& vec3::operator+=(float scalar)
    {
        return *this += vec3(scalar);
    }
    __host__ __device__ vec3 vec3::operator+(const vec3& vector) const
    {
        vec3 out;
        for (int i = 0; i < 3; i++)
        {
            out[i] = *(&x + i) + vector[i];
        }
        return out;
    }
    __host__ __device__ vec3& vec3::operator+=(const vec3& vector)
    {
        for (int i = 0; i < 3; i++)
        {
            *(&x + i) += vector[i];
        }
        return *this;
    }

    __host__ __device__ vec3 vec3::operator-(float scalar) const
    {
        return *this - vec3(scalar);
    }
    __host__ __device__ vec3& vec3::operator-=(float scalar)
    {
        return *this -= vec3(scalar);
    }
    __host__ __device__ vec3 vec3::operator-(const vec3& vector) const
    {
        vec3 out;
        for (int i = 0; i < 3; i++)
        {
            out[i] = *(&x + i) - vector[i];
        }
        return out;
    }
    __host__ __device__ vec3& vec3::operator-=(const vec3& vector)
    {
        for (int i = 0; i < 3; i++)
        {
            *(&x + i) -= vector[i];
        }
        return *this;
    }

    __host__ __device__ vec3 vec3::operator*(float scalar) const
    {
        vec3 out;
        for (int i = 0; i < 3; i++)
        {
            out[i] = *(&x + i) * scalar;
        }
        return out;
    }
    __host__ __device__ vec3& vec3::operator*=(float scalar)
    {
        for (int i = 0; i < 3; i++)
        {
            *(&x + i) *= scalar;
        }
        return *this;
    }

    __host__ __device__ vec3 vec3::operator/(float scalar) const
    {
        vec3 out;
        for (int i = 0; i < 3; i++)
        {
            out[i] = *(&x + i) / scalar;
        }
        return out;
    }
    __host__ __device__ vec3& vec3::operator/=(float scalar)
    {
        for (int i = 0; i < 3; i++)
        {
            *(&x + i) /= scalar;
        }
        return *this;
    }

    __host__ std::ostream& operator << (std::ostream& os, const vec3& vector)
    {
        return os << "[" << vector[0] << ", " << vector[1] << ", " << vector[2] << "]";
    }
}