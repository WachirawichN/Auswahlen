#pragma once

#include <string>
#include <unordered_map>

#include "../dependencies/GLM/glm.hpp"

struct shaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

class shader
{
    private:
        std::string filePath;
        unsigned int shaderID;
        std::unordered_map<std::string, int> uniformLocationCache;

        shaderProgramSource parseShader(const std::string& filePath);
        unsigned int compileShader(unsigned int type, const std::string& source);
        unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

        int getUniformLocation(const std::string& name);
    public:
        shader(const std::string& filepath);
        ~shader();

        void bind() const;
        void unbind() const;

        void setUniform1i(const std::string& name, int value);
        void setUniform1f(const std::string& name, float value);
        void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
        void setUniformMat4fv(const std::string& name, unsigned char transpose, glm::mat4 transform);
};