#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    // the program ID
    uint32_t ID;
  
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath=nullptr);
    ~Shader();
    
    // use/activate the shader
    void use();

    // utility uniform functions
    void setBool (const std::string &name, bool value) const;  
    void setInt  (const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setVec2 (const std::string &name, float x, float y) const;
    void setVec3 (const std::string &name, float x, float y, float z) const;
    void setVec4 (const std::string &name, float x, float y, float z, float w) const;
    void setVec2 (const std::string &name, const glm::vec2& value) const;
    void setVec3 (const std::string &name, const glm::vec3& value) const;
    void setVec4 (const std::string &name, const glm::vec4& value) const;
    void setMat3 (const std::string &name, const glm::mat3& value) const;
    void setMat2 (const std::string &name, const glm::mat2& value) const;
    void setMat4 (const std::string &name, const glm::mat4& value) const;

private:
    //! link shader program
    bool link(const char * vsCode, const char * fsCode, const char * gsCode=nullptr);
    void checkCompileErrors(uint32_t shader, std::string type) const;
};