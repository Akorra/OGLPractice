#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

#include <string>
#include <vector>

inline constexpr uint8_t kMaxBoneInfluence = 4;

struct Vertex 
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 biTangent;

    // bone indexes that influencce this vertex
    int m_BoneIDs[kMaxBoneInfluence];
    // weights from each bone
    float m_Weights[kMaxBoneInfluence];
};

struct Texture
{
    uint32_t id;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> v, std::vector<uint32_t> i, std::vector<Texture> t) 
        : vertices(std::move(v)), indices(std::move(i)), textures(std::move(t)) { setupMesh(); }
    ~Mesh() = default;

    void draw(Shader &shader) 
    {
        uint32_t diffuseNr = 1, specularNr = 1, normalNr = 1, heightNr = 1;
        for(uint32_t i = 0; i<textures.size(); ++i)
        {
            glActiveTexture(GL_TEXTURE0 + i); //< active proper texture unit

            std::string number;
            std::string name = textures[i].type;
            if(name == "texture_diffuse")       number = std::to_string(diffuseNr++);
            else if(name == "texture_specular") number = std::to_string(specularNr++);
            else if(name == "texture_normal")   number = std::to_string(normalNr++);
            else if(name == "texture_height")   number = std::to_string(heightNr++);

            // set sampler to correct texture unit
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<uint32_t>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

private:
    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // bind vao - record mesh bindings
        glBindVertexArray(VAO);
        
        // load data into vbo
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // structs - sequential data layout:
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        
        // now for indices:
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);

        // vertex attributes:
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        // texture coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, biTangent));

        // ids
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, kMaxBoneInfluence, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        // weights
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, kMaxBoneInfluence, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

        // unbind vao
        glBindVertexArray(0);
    }

private:
    uint32_t VAO, VBO, EBO;

public:
    // mesh data
    std::vector<Vertex>   vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture>  textures;
};