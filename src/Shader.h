#pragma once

#include <string>
#include <unordered_map>

#include "glm/gtc/matrix_transform.hpp"


/* Struct which stores vertex and fragment shader source code. */
struct ShaderSources
{
    std::string vertex_source;
    std::string fragment_source;
};
class Shader
{
private:
    std::unordered_map<std::string, int> m_LocationCache;
    std::string m_FilePath;
    unsigned int m_RendererAddress;
public:
    Shader(const std::string& filepath);
    ~Shader();
    
    void Bind() const;
    void Unbind() const;

    void SetUniform1i(const std::string& name, int value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);
private:
    ShaderSources LoadShader(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& filepath);

    int GetUniformLocation(const std::string& name);
};