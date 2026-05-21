#include "shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "lighthelper.hpp"

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    uint32_t vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

// activate the shader
void Shader::use()
{
    glUseProgram(ID);
}

// utility uniform functions
void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat2(const std::string &name, const glm::vec2& v) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), v.x, v.y);
}

void Shader::setFloat3(const std::string &name, const glm::vec3& v) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), v.x, v.y, v.z);
}

void Shader::setFloat4(const std::string &name, const glm::vec4& v) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), v.x, v.y, v.z, v.w);
}

void Shader::setMat3(const std::string &name, const glm::mat3& transform) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(transform));
}

void Shader::setMat4(const std::string &name, const glm::mat4& transform) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(transform));
}

void Shader::addDirectionalLight(const std::string &name, DirectionalLight* light) const
{
    setFloat3(name + ".direction", light->direction);
    setFloat3(name + ".ambient",   light->ambient);
    setFloat3(name + ".diffuse",   light->diffuse);
    setFloat3(name + ".specular",  light->specular);
}

void Shader::addPointLight(const std::string &name, PointLight* light) const
{
    setFloat3(name + ".position", light->position);
    setFloat3(name + ".ambient",  light->ambient);
    setFloat3(name + ".diffuse",  light->diffuse);
    setFloat3(name + ".specular", light->specular);
    setFloat(name + ".constant",  light->constant);
    setFloat(name + ".linear",    light->linear);
    setFloat(name + ".quadratic", light->quadratic);
}

void Shader::addSpotLight(const std::string &name, SpotLight* light) const
{
    setFloat3(name + ".position",   light->position);
    setFloat3(name + ".direction",  light->direction);
    setFloat3(name + ".ambient",    light->ambient);
    setFloat3(name + ".diffuse",    light->diffuse);
    setFloat3(name + ".specular",   light->specular);
    setFloat(name + ".constant",    light->constant);
    setFloat(name + ".linear",      light->linear);
    setFloat(name + ".quadratic",   light->quadratic);
    setFloat(name + ".cutoff",      light->innerCutoff);
    setFloat(name + ".outerCutoff", light->outerCutoff);
}

void Shader::checkCompileErrors(uint32_t shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } 
}
