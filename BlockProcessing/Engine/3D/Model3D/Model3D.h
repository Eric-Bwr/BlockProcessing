#pragma once

#include "Mesh3D.h"
#include <vector>
#include <memory>

class Model3D {
public:
    explicit Model3D(const char* path, const char* textureFolderPath = "", const unsigned int drawMode = GL_TRIANGLES);
    explicit Model3D(const float* vertices, const uint64_t verticesSize, const unsigned int drawMode = GL_TRIANGLES);
    explicit Model3D(const float* vertices, const uint64_t verticesSize, const unsigned int* indices, const uint64_t indicesSize, const unsigned int drawMode = GL_TRIANGLES);
    explicit Model3D(const std::vector<float> &vertices, const unsigned int drawMode = GL_TRIANGLES);
    explicit Model3D(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, const unsigned int drawMode = GL_TRIANGLES);
    void setupMaterial(Shader& shader);
    void renderMaterialColor(Shader& shader);
    void renderMaterialTexture(Shader& shader);
    void renderMaterial(Shader& shader);
    void render();
    void draw();
    void bind();
    void unbind();
    void setUniforms(Uniforms& uniforms);
    ~Model3D();
private:
    Texture* getTexture(const char* path);
    std::vector<Mesh3D*> meshes;
    std::vector<Texture*> loadedTextures;
};
