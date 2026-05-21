#pragma once

#include "mesh.hpp"
#include <assimp/material.h>

class Model
{
public:
    Model(const std::string &path, bool gamma = false);
    ~Model() = default;

    void Draw(Shader& shader);

private:
    //! loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void LoadModel(const std::string &path);

    //! processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void ProcessNode(struct aiNode *node, const struct aiScene *scene);

    //! loads to our mesh type
    Mesh ProcessMesh(struct aiMesh *mesh, const struct aiScene *scene);

    //! checks all material textures of a given type and loads the textures if they're not loaded yet. the required info is returned as a Texture struct.
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

public:
    std::vector<Texture> texturesLoaded;
    std::vector<Mesh>    meshes;
    std::string          directory;
    bool                 gammaCorrection;
};