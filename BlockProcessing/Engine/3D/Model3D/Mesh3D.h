#pragma once

#include <Buffer.h>
#include <Texture.h>
#include <Shader.h>

struct Material {
    float diffuseX, diffuseY, diffuseZ,
    specularX, specularY, specularZ,
    emissiveX, emissiveY, emissiveZ,
    shininess;
    Texture* diffuseTexture = nullptr;
    Texture* normalTexture = nullptr;
    Texture* specularTexture = nullptr;
    Texture* ambientTexture = nullptr;
    Texture* displacementTexture = nullptr;
};

struct Uniforms {
    const char* diffuse = "materialColorDiffuse";
    const char* specular = "materialColorSpecular";
    const char* emissive = "materialColorEmissive";
    const char* shininess = "materialShininess";
    const char* diffuseTexture = "materialTextureDiffuse";
    const char* normalTexture = "materialTextureNormal";
    const char* specularTexture = "materialTextureSpecular";
    const char* ambientTexture = "materialTextureAmbient";
    const char* displacementTexture = "materialTextureDisplacement";
    int diffuseTextureLocation = 0;
    int normalTextureLocation = 1;
    int specularTextureLocation = 2;
    int ambientTextureLocation = 3;
    int displacementTextureLocation = 4;
};

class Mesh3D {
public:
    explicit Mesh3D(const float* vertices, const uint64_t verticesSize, const unsigned int* indices, const uint64_t indicesSize, unsigned int drawType = GL_TRIANGLES, unsigned int drawMode = GL_STATIC_DRAW);
    explicit Mesh3D(const float* vertices, const uint64_t verticesSize, unsigned int drawType = GL_TRIANGLES, unsigned int drawMode = GL_STATIC_DRAW);
    void changeVertices(const float* vertices, const uint64_t verticesSize, unsigned int offset, unsigned int drawMode);
    void changeIndices(const float* indices, const uint64_t indicesSize, unsigned int offset, unsigned int drawMode);
    void setMaterial(Material& material);
    void renderMaterialColor(Shader* shader);
    void renderMaterialTexture(Shader* shader);
    void renderMaterial(Shader* shader);
    void render();
    void draw();
    void bind();
    static void unbind();
    inline Uniforms* getUniforms() { return uniformNames; }
    inline void setUniforms(Uniforms* uniforms){ this->uniformNames = uniforms; }
    ~Mesh3D();
private:
    VertexArrayObject* vao;
    VertexBufferObject* vbo;
    IndicesBufferObject* ibo;
    Uniforms* uniformNames;
    Material material;
    uint64_t numIndices = 0;
    uint64_t numVertices = 0;
    unsigned int drawType;
};