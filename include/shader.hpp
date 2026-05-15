#pragma once

#include <glad/gl.h> // include glad to get all the required OpenGL headers
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

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
    void setFloat2(const std::string &name, float x, float y) const;
    void setFloat3(const std::string &name, float x, float y, float z) const;
    void setFloat4(const std::string &name, float x, float y, float z, float w) const;
    void setTexture(const std::string &name, const std::string &path) const;

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};