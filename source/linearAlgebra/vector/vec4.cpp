#include "vector.h"

namespace vector
{
    vec4::vec4()
    {
        value = (float*)malloc(sizeof(float) * 4);

        value[0] = 0.0f;
        value[1] = 0.0f;
        value[2] = 0.0f;
        value[3] = 0.0f;
    }
    vec4::vec4(float v0)
    {
        value = (float*)malloc(sizeof(float) * 4);

        value[0] = v0;
        value[1] = v0;
        value[2] = v0;
        value[3] = v0;
    }
    vec4::vec4(float v0, float v1, float v2, float v3)
    {
        value = (float*)malloc(sizeof(float) * 4);

        value[0] = v0;
        value[1] = v1;
        value[2] = v2;
        value[3] = v3;
    }
    vec4::~vec4()
    {
        free(value);
    }


    float vec4::operator[](unsigned int index) const
    {
        return value[index];
    }
    indexProxy vec4::operator[](unsigned int index)
    {
        return indexProxy(value[index]);
    }
    vec4& vec4::operator=(const vec4& input)
    {
        value[0] = input[0];
        value[1] = input[1];
        value[2] = input[2];
        value[3] = input[3];
        return *this;
    }


    vec4 vec4::operator+(vec4 input)
    {
        return vec4(value[0] + input[0], value[1] + input[1], value[2] + input[2], value[3] + input[3]);
    }
    vec4& vec4::operator+=(vec4& input)
    {
        value[0] += input[0];
        value[1] += input[1];
        value[2] += input[2];
        value[3] += input[3];
        return *this;
    }

    vec4 vec4::operator-(vec4 input)
    {
        return vec4(value[0] - input[0], value[1] - input[1], value[2] - input[2], value[3] - input[3]);
    }
    vec4& vec4::operator-=(vec4& input)
    {
        value[0] -= input[0];
        value[1] -= input[1];
        value[2] -= input[2];
        value[3] -= input[3];
        return *this;
    }

    vec4 vec4::operator*(float scalar)
    {
        return vec4(value[0] * scalar, value[1] * scalar, value[2] * scalar, value[3] * scalar);
    }
    vec4& vec4::operator*=(float scalar)
    {
        value[0] *= scalar;
        value[1] *= scalar;
        value[2] *= scalar;
        value[3] *= scalar;
        return *this;
    }

    vec4 vec4::operator/(float scalar)
    {
        return vec4(value[0] / scalar, value[1] / scalar, value[2] / scalar, value[3] / scalar);
    }
    vec4& vec4::operator/=(float scalar)
    {
        value[0] /= scalar;
        value[1] /= scalar;
        value[2] /= scalar;
        value[3] /= scalar;
        return *this;
    }


    glm::vec4 vec4::toGLM() const
    {
        return glm::vec4(value[0], value[1], value[2], value[3]);
    }
}