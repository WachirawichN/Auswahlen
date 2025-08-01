#include <GLC/matrix.cuh>

namespace GLC
{
    __host__ __device__ vec4& mat4::operator[](unsigned int index)
    {
        return *(&x + index);
    }
    __host__ __device__ const vec4& mat4::operator[](unsigned int index) const
    {
        return *(&x + index);
    }

    __host__ __device__ mat4 mat4::operator+(const mat4& matrix) const
    {
        mat4 out;
        for (int i = 0; i < 4; ++i)
        {
            out[i] = (*this)[i] + matrix[i];
        }
        return out;
    }
    __host__ __device__ mat4& mat4::operator+=(const mat4& matrix)
    {
        for (int i = 0; i < 4; ++i)
        {
            (*this)[i] += matrix[i];
        }
        return *this;
    }

    __host__ __device__ mat4 mat4::operator-(const mat4& matrix) const
    {
        mat4 out;
        for (int i = 0; i < 4; ++i)
        {
            out[i] = (*this)[i] - matrix[i];
        }
        return out;
    }
    __host__ __device__ mat4& mat4::operator-=(const mat4& matrix)
    {
        for (int i = 0; i < 4; ++i)
        {
            (*this)[i] -= matrix[i];
        }
        return *this;
    }

    __host__ __device__ mat4 mat4::operator*(float scalar) const
    {
        mat4 out;
        for (int i = 0; i < 4; ++i)
        {
            out[i] = (*this)[i] * scalar;
        }
        return out;
    }
    __host__ __device__ mat4& mat4::operator*=(float scalar)
    {
        for (int i = 0; i < 4; ++i)
        {
            (*this)[i] *= scalar;
        }
        return *this;
    }
    __host__ __device__ mat4 mat4::operator*(const mat4& matrix) const
    {
        mat4 out;
        mat4 transposed = transpose(*this);
        for (int column = 0; column < 4; column++)
        {
            for (int row = 0; row < 4; row++)
            {
                out[row][column] = dot(transposed[column], matrix[row]);
            }
        }
        return out;
    }
    __host__ __device__ mat4& mat4::operator*=(const mat4& matrix)
    {
        mat4 transposed = transpose(*this);
        for (int column = 0; column < 4; column++)
        {
            for (int row = 0; row < 4; row++)
            {
                (*this)[row][column] = dot(transposed[column], matrix[row]);
            }
        }
        return *this;
    }
    __host__ __device__ vec4 mat4::operator*(const vec4& vector) const
    {
        vec4 out;
        mat4 transposed = transpose(*this);
        for (int row = 0; row < 4; row++)
        {
            for (int column = 0; column < 4; column++)
            {
                out[row] += transposed[row][column] * vector[column];
            }
        }
        return out;
    }

    __host__ __device__ mat4 mat4::operator/(float scalar) const
    {
        mat4 out;
        for (int i = 0; i < 4; ++i)
        {
            out[i] = (*this)[i] / scalar;
        }
        return out;
    }
    __host__ __device__ mat4& mat4::operator/=(float scalar)
    {
        for (int i = 0; i < 4; ++i)
        {
            (*this)[i] /= scalar;
        }
        return *this;
    }

    __host__ std::ostream& operator<<(std::ostream& os, const mat4& matrix)
    {
        // Expected output
        // |                         |
        // | | a | | e | | i | | m | |
        // | | b | | f | | j | | n | |
        // | | c | | g | | k | | o | |
        // | | d | | h | | l | | p | |
        // |                         |

        // Check for maximum length of every number inside matrix
        unsigned int maxLength = 0;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (std::to_string(matrix[i][j]).length() > maxLength)
                {
                    maxLength = std::to_string(matrix[i][j]).length();
                }
            }
        }

        for (int row = 0; row < 6; row++)
        {
            os << "|" << " "; 

            for (int column = 0; column < 4; column++)
            {
                std::string bracket;
                std::string numStr;
                if (row == 0 || row == 5)
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