#pragma once

#include <glm/glm.hpp>

struct DirectionalLight 
{
    glm::vec3 direction = {-0.2f, -1.0f, -0.3f};
    glm::vec3 ambient   = {0.05f, 0.05f, 0.05f};
    glm::vec3 diffuse   = {0.4f, 0.4f, 0.4f};
    glm::vec3 specular  = {0.5f, 0.5f, 0.5f};
};

struct PointLight 
{    
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    glm::vec3 ambient  = {0.05f, 0.05f, 0.05f};
    glm::vec3 diffuse  = {0.8f, 0.8f, 0.8f};
    glm::vec3 specular = {1.0f, 1.0f, 1.0f};
    
    // attenuation
    float constant  = 1.0f;
    float linear    = 0.09f;
    float quadratic = 0.032;  
};  

struct SpotLight
{
    glm::vec3 position  = {0.0f, 0.0f, 0.0f};
    glm::vec3 direction = {0.0f, 0.0f, -1.0f}; 
    glm::vec3 ambient   = {0.0f, 0.0f, 0.0f};
    glm::vec3 diffuse   = {1.0f, 1.0f, 1.0f};
    glm::vec3 specular  = {1.0f, 1.0f, 1.0f}; 
    
    // attenuation
    float constant  = 1.0f;
    float linear    = 0.09f;
    float quadratic = 0.032f; 

    float innerCutoff = glm::cos(glm::radians(12.5f));
    float outerCutoff = glm::cos(glm::radians(15.0f));
};

inline constexpr glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};