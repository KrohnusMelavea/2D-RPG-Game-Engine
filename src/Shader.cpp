#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "Renderer.h"
#include "ConsoleMagic.h"


/* Streams shader source code from file into "ShaderSources" struct. */
ShaderSources Shader::LoadShader(const std::string& filepath)
{
    std::ifstream inFile(filepath);
    if (inFile.is_open())
    {
        std::cout << "Opened \"" << filepath << "\" Successfully" << std::endl;
    }
    else
    {
        std::cout << "Invalid Filepath: \"" << filepath << "\"" << std::endl;
    }

    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
    ShaderType type = ShaderType::NONE;
    std::stringstream streams[2];
    std::string line;
    while (getline(inFile, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            streams[(int)type] << line.append("\n");
        }
    }

    return { streams[0].str(), streams[1].str() };
}
/* Compiles shader source code and returns address to shader. */
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int shader_address = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(shader_address, 1, &src, nullptr));
    GLCall(glCompileShader(shader_address));

    int result;
    GLCall(glGetShaderiv(shader_address, GL_COMPILE_STATUS, &result));
    if (!result)
    {
        int length;
        GLCall(glGetShaderiv(shader_address, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)_malloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(shader_address, length, &length, message));
        std::cout << ((type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment") << " Shader Compilation Error:" << std::endl;
        std::cout << message << std::endl;
    }

    return shader_address;
}
/* Creates shader program and returns its address. */
unsigned int Shader::CreateShader(const std::string& filepath)
{
    GLCall(unsigned int program_address = glCreateProgram());

    ShaderSources sources = LoadShader(filepath);
    outputShader(sources.vertex_source, sources.fragment_source);
    unsigned int vertex_shader_address = CompileShader(GL_VERTEX_SHADER, sources.vertex_source);
    unsigned int fragment_shader_address = CompileShader(GL_FRAGMENT_SHADER, sources.fragment_source);

    //Binds shaders to shader program.
    GLCall(glAttachShader(program_address, vertex_shader_address));
    GLCall(glAttachShader(program_address, fragment_shader_address));

    GLCall(glLinkProgram(program_address));
    GLCall(glValidateProgram(program_address));

    //Detach the shaders to save a few KB of vram (better to keep them attached for debugging purposes).
    //GLCall(glDetachShader(program_address, vertex_shader_address);
    //GLCall(glDetachShader(program_address, fragment_shader_address);

    //Delete the shaders as they've already been attached and are no longer necessary.
    GLCall(glDeleteShader(vertex_shader_address));
    GLCall(glDeleteShader(fragment_shader_address));

    return program_address;
}

Shader::Shader(const std::string& filepath)
: m_RendererAddress(0), m_FilePath(filepath)
{
    m_RendererAddress = CreateShader(filepath);
}
Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererAddress));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererAddress));
}
void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}
void Shader::SetUniformMat4f(const std::string& name, const glm::mat4 matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}
int Shader::GetUniformLocation(const std::string& name)
{
    if (m_LocationCache.find(name) != m_LocationCache.end()) return m_LocationCache[name];
    GLCall(int location = glGetUniformLocation(m_RendererAddress, name.c_str()));
    if (location == -1)
    {
        std::cout << "Invalid Uniform: " << name << std::endl;
    }
    m_LocationCache[name] = location;
    return location;
}