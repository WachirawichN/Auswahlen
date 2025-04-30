#include <GLC/matrix.cuh>

namespace GLC
{
    __host__ __device__ mat3::mat3()
    {
        for (int i = 0; i < 3; i++)
        {
            value[i] = vec3();
        }
    }
    __host__ __device__ mat3::mat3(float v0)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (i == j) value[i][j] = v0;
            }
        }
    }
    __host__ __device__ mat3::mat3(const vec3& v0, const vec3& v1, const vec3& v2)
    {
        value[0] = v0;
        value[1] = v1;
        value[2] = v2;
    }

    __host__ __device__ vec3& mat3::operator[](unsigned int index)
    {
        return value[index];
    }
    __host__ __device__ const vec3& mat3::operator[] (unsigned int index) const
    {
        return value[index];
    }
    __host__ __device__ mat3& mat3::operator=(const mat3& matrix)
    {
        if (this != &matrix)
        {
            for (int i = 0; i < 3; i++)
            {
                value[i] = matrix[i];
            }
        }
        return *this;
    }

    __host__ __device__ mat3 mat3::operator+(const mat3& matrix) const
    {
        mat3 out;
        for (int i = 0; i < 3; ++i)
        {
            out[i] = value[i] + matrix[i];
        }
        return out;
    }
    __host__ __device__ mat3& mat3::operator+=(const mat3& matrix)
    {
        for (int i = 0; i < 3; ++i)
        {
            value[i] += matrix[i];
        }
        return *this;
    }

    __host__ __device__ mat3 mat3::operator-(const mat3& matrix) const
    {
        mat3 out;
        for (int i = 0; i < 3; ++i)
        {
            out[i] = value[i] - matrix[i];
        }
        return out;
    }
    __host__ __device__ mat3& mat3::operator-=(const mat3& matrix)
    {
        for (int i = 0; i < 3; ++i)
        {
            value[i] -= matrix[i];
        }
        return *this;
    }

    __host__ __device__ mat3 mat3::operator*(float scalar) const
    {
        mat3 out;
        for (int i = 0; i < 3; ++i)
        {
            out[i] = value[i] * scalar;
        }
        return out;
    }
    __host__ __device__ mat3& mat3::operator*=(float scalar)
    {
        for (int i = 0; i < 3; ++i)
        {
            value[i] *= scalar;
        }
        return *this;
    }
    __host__ __device__ mat3 mat3::operator*(const mat3& matrix) const
    {
        mat3 out;
        mat3 transposed = transpose(*this);
        for (int column = 0; column < 3; column++)
        {
            for (int row = 0; row < 3; row++)
            {
                out[row][column] = dot(transposed[column], matrix[row]);
            }
        }
        return out;
    }
    __host__ __device__ mat3& mat3::operator*=(const mat3& matrix)
    {
        mat3 transposed = transpose(*this);
        for (int column = 0; column < 3; column++)
        {
            for (int row = 0; row < 3; row++)
            {
                value[row][column] = dot(transposed[column], matrix[row]);
            }
        }
        return *this;
    }
    __host__ __device__ vec3 mat3::operator*(const vec3& vector) const
    {
        vec3 out;
        mat3 transposed = transpose(*this);
        for (int row = 0; row < 3; row++)
        {
            for (int column = 0; column < 3; column++)
            {
                out[row] += transposed[row][column] * vector[column];
            }
        }
        return out;
    }

    __host__ __device__ mat3 mat3::operator/(float scalar) const
    {
        mat3 out;
        for (int i = 0; i < 3; ++i)
        {
            out[i] = value[i] / scalar;
        }
        return out;
    }
    __host__ __device__ mat3& mat3::operator/=(float scalar)
    {
        for (int i = 0; i < 3; ++i)
        {
            value[i] /= scalar;
        }
        return *this;
    }

    __host__ std::ostream& operator<<(std::ostream& os, const mat3& matrix)
    {
        // Expected output
        // |                   |
        // | | a | | d | | g | |
        // | | b | | e | | h | |
        // | | c | | f | | i | |
        // |                   |

        // Check for maximum length of every number inside matrix
        unsigned int maxLength = 0;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (std::to_string(matrix[i][j]).length() > maxLength)
                {
                    maxLength = std::to_string(matrix[i][j]).length();
                }
            }
        }

        for (int row = 0; row < 5; row++)
        {
            os << "|" << " "; 

            for (int column = 0; column < 3; column++)
            {
                std::string bracket;
                std::string numStr;
                if (row == 0 || row == 4)
                {
                    bracket = " ";
                    numStr = std::string(maxLength, ' ');
                }
                else
                {
                    bracket = "|";
                    numStr = std::string(maxLength - std::to_string(matrix[column][row - 1]).length(), ' ') + std::to_string(matrix[column][row - 1]);
                }
                os << bracket << " " << numStr << " " << bracket << " ";
            }

            os << "|" << "\n";
        }
        return os;
    }
}