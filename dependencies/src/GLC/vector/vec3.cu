#include "../../include/vector.cuh"

namespace GLC
{
    __host__ __device__ vec3::vec3()
    {
        for (int i = 0; i < 3; i++)
        {
            value[i] = 0.0f;
        }
    }
    __host__ __device__ vec3::vec3(float v0)
    {
        for (int i = 0; i < 3; i++)
        {
            value[i] = v0;
        }
    }
    __host__ __device__ vec3::vec3(float v0, float v1, float v2)
    {
        value[0] = v0;
        value[1] = v1;
        value[2] = v2;
    }
    __host__ __device__ vec3::vec3(const vec3& vector)
    {
        for (int i = 0; i < 3; i++)
        {
            value[i] = vector[i];
        }
    }

    __host__ __device__ float& vec3::operator[](unsigned int index)
    {
        return value[index];
    }
    __host__ __device__ float vec3::operator[](unsigned int index) const
    {
        return value[index];
    }
    __host__ __device__ vec3& vec3::operator=(const vec3& vector)
    {
        if (this != &vector)
        {
            for (int i = 0; i < 3; i++)
            {
                value[i] = vector[i];
            }
        }
        return *this;
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
            out[i] = value[i] + vector[i];
        }
        return out;
    }
    __host__ __device__ vec3& vec3::operator+=(const vec3& vector)
    {
        for (int i = 0; i < 3; i++)
        {
            value[i] += vector[i];
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
            out[i] = value[i] - vector[i];
        }
        return out;
    }
    __host__ __device__ vec3& vec3::operator-=(const vec3& vector)
    {
        for (int i = 0; i < 3; i++)
        {
            value[i] -= vector[i];
        }
        return *this;
    }

    __host__ __device__ vec3 vec3::operator*(float scalar) const
    {
        vec3 out;
        for (int i = 0; i < 3; i++)
        {
            out[i] = value[i] * scalar;
        }
        return out;
    }
    __host__ __device__ vec3& vec3::operator*=(float scalar)
    {
        for (int i = 0; i < 3; i++)
        {
            value[i] *= scalar;
        }
        return *this;
    }

    __host__ __device__ vec3 vec3::operator/(float scalar) const
    {
        vec3 out;
        for (int i = 0; i < 3; i++)
        {
            out[i] = value[i] / scalar;
        }
        return out;
    }
    __host__ __device__ vec3& vec3::operator/=(float scalar)
    {
        for (int i = 0; i < 3; i++)
        {
            value[i] /= scalar;
        }
        return *this;
    }

    __host__ std::ostream& operator << (std::ostream& os, const vec3& vector)
    {
        return os << "[" << vector[0] << ", " << vector[1] << ", " << vector[2] << "]";
    }
}