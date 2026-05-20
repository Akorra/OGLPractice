#pragma once

#include <glad/gl.h> // include glad to get all the required OpenGL headers
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>

class Shader
{
public:
    // the program ID
    unsigned int ID;
  
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setFloat2(const std::string &name, const glm::vec2& v) const;
    void setFloat3(const std::string &name, const glm::vec3& v) const;
    void setFloat4(const std::string &name, const glm::vec4& v) const;
    void setMat3(const std::string &name, const glm::mat3& transform) const;
    void setMat4(const std::string &name, const glm::mat4& transform) const;

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};