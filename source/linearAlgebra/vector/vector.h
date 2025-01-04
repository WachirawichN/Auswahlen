#pragma once

#include "../../dependencies/GLM/glm.hpp"

#include <iostream>

namespace vector
{
    class indexProxy
    {
        private:
            float& ref;
        public:
            indexProxy(float& input);

            indexProxy& operator += (float input);
            indexProxy& operator -= (float input);
            indexProxy& operator *= (float input);
            indexProxy& operator /= (float input);

            operator float() const;
    };

    class vec3
    {
        private:
            float* value;
        public:
            vec3();
            vec3(float v0);
            vec3(float v0, float v1, float v2);
            ~vec3();


            float operator [] (unsigned int index) const;
            indexProxy operator [] (unsigned int index);
            vec3& operator = (const vec3& input);

            vec3 operator + (vec3 input);
            vec3& operator += (vec3& input);

            vec3 operator - (vec3 input);
            vec3& operator -= (vec3& input);

            vec3 operator * (float scalar);
            vec3& operator *= (float scalar);

            vec3 operator / (float scalar);
            vec3& operator /= (float scalar);

            glm::vec3 toGLM() const;
    };

    class vec4
    {
        private:
            float* value;
        public:
            vec4();
            vec4(float v0);
            vec4(float v0, float v1, float v2, float v3);
            ~vec4();


            float operator [] (unsigned int index) const;
            indexProxy operator [] (unsigned int index);
            vec4& operator = (const vec4& input);

            vec4 operator + (vec4 input);
            vec4& operator += (vec4& input);

            vec4 operator - (vec4 input);
            vec4& operator -= (vec4& input);

            vec4 operator * (float scalar);
            vec4& operator *= (float scalar);

            vec4 operator / (float scalar);
            vec4& operator /= (float scalar);

            glm::vec4 toGLM() const;
    };
}