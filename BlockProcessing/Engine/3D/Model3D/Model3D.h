#pragma once

#include "Mesh3D.h"
#include <Util/Vector.h>
#include <EventSystem/EventSystem.h>
#include <CallbackSystem/Callback.h>
#include <memory>

class Model3D {
public:
    explicit Model3D(const char* path, const char* textureFolderPath = "", const unsigned int drawMode = GL_TRIANGLES);
    explicit Model3D(const float* vertices, const uint64_t verticesSize, const unsigned int drawMode = GL_TRIANGLES);
    explicit Model3D(const float* vertices, const uint64_t verticesSize, const unsigned int* indices, const uint64_t indicesSize, const unsigned int drawMode = GL_TRIANGLES);
    explicit Model3D(const Vector<float> &vertices, const unsigned int drawMode = GL_TRIANGLES);
    explicit Model3D(const Vector<float> &vertices, const Vector<unsigned int> &indices, const unsigned int drawMode = GL_TRIANGLES);
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
    static void setEventCallback(std::shared_ptr<cs::BasicCallback<void, es::Event&>>& callback) { eventCallback = callback; }
private:
    Texture* getTexture(const char* path);
    Vector<Mesh3D*> meshes;
    Vector<Texture*> loadedTextures;
    static std::shared_ptr<cs::BasicCallback<void, es::Event&>> eventCallback;
};
