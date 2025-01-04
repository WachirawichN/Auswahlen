#include "vector.h"

namespace vector
{
    vec3::vec3()
    {
        value = (float*)malloc(sizeof(float) * 3);

        value[0] = 0.0f;
        value[1] = 0.0f;
        value[2] = 0.0f;
    }
    vec3::vec3(float v0)
    {
        value = (float*)malloc(sizeof(float) * 3);

        value[0] = v0;
        value[1] = v0;
        value[2] = v0;
    }
    vec3::vec3(float v0, float v1, float v2)
    {
        value = (float*)malloc(sizeof(float) * 3);

        value[0] = v0;
        value[1] = v1;
        value[2] = v2;
    }
    vec3::~vec3()
    {
        free(value);
    }


    float vec3::operator[](unsigned int index) const
    {
        return value[index];
    }
    indexProxy vec3::operator[](unsigned int index)
    {
        return indexProxy(value[index]);
    }
    vec3& vec3::operator=(const vec3& input)
    {
        value[0] = input[0];
        value[1] = input[1];
        value[2] = input[2];
        return *this;
    }


    vec3 vec3::operator+(vec3 input)
    {
        return vec3(value[0] + input[0], value[1] + input[1], value[2] + input[2]);
    }
    vec3& vec3::operator+=(vec3& input)
    {
        value[0] += input[0];
        value[1] += input[1];
        value[2] += input[2];
        return *this;
    }

    vec3 vec3::operator-(vec3 input)
    {
        return vec3(value[0] - input[0], value[1] - input[1], value[2] - input[2]);
    }
    vec3& vec3::operator-=(vec3& input)
    {
        value[0] -= input[0];
        value[1] -= input[1];
        value[2] -= input[2];
        return *this;
    }

    vec3 vec3::operator*(float scalar)
    {
        return vec3(value[0] * scalar, value[1] * scalar, value[2] * scalar);
    }
    vec3& vec3::operator*=(float scalar)
    {
        value[0] *= scalar;
        value[1] *= scalar;
        value[2] *= scalar;
        return *this;
    }

    vec3 vec3::operator/(float scalar)
    {
        return vec3(value[0] / scalar, value[1] / scalar, value[2] / scalar);
    }
    vec3& vec3::operator/=(float scalar)
    {
        value[0] /= scalar;
        value[1] /= scalar;
        value[2] /= scalar;
        return *this;
    }


    glm::vec3 vec3::toGLM() const
    {
        return glm::vec3(value[0], value[1], value[2]);
    }
}