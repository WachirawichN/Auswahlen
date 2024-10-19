#include "shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "renderer.h"

shader::shader(const std::string& filepath)
    : filePath(filepath), shaderID(0)
{
    shaderProgramSource source = parseShader(filepath);
    shaderID =  createShader(source.vertexSource, source.fragmentSource);
}

shader::~shader()
{
    glDeleteProgram(shaderID);
}

shaderProgramSource shader::parseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    enum class shaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream shaderStrings[2];
    shaderType type = shaderType::NONE;
    // Check if each line contain word "#shader"
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = shaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = shaderType::FRAGMENT;
            }
        }
        else
        {
            shaderStrings[(int)type] << line << "\n";
        }
    }
    return { shaderStrings[0].str(), shaderStrings[1].str()};
}

unsigned int shader::compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // Convert to C-style string
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); // Allocate on stack dynamicly
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program =  glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Linking both shader into one shader
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    // Delete vertex and fragment shader
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

void shader::bind() const
{
    glUseProgram(shaderID);
}

void shader::unbind() const
{
    glUseProgram(0);
}

void shader::setUniform1i(const std::string& name, int value)
{
    glUniform1i(getUniformLocation(name), value);
}

void shader::setUniform1f(const std::string& name, float value)
{
    glUniform1f(getUniformLocation(name), value);
}

void shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

int shader::getUniformLocation(const std::string& name)
{
    // Add the name into chache to provide a bit more performance.
    // Check if m_UniformLocationCache contain its name.
    if (uniformLocationCache.find(name) != uniformLocationCache.end())
    {
        return uniformLocationCache[name];
    }

    int location = glGetUniformLocation(shaderID, name.c_str());
    if (location == -1)
    {
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }
    uniformLocationCache[name] = location;
    return location;
}