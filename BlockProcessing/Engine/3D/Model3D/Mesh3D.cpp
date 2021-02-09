#include "Mesh3D.h"

Mesh3D::Mesh3D(const float* vertices, const uint64_t verticesSize, unsigned int drawType, unsigned int drawMode) : numVertices(verticesSize / 14), drawType(drawType) {
    auto layout = new VertexBufferObjectLayout();
    layout->pushFloat(3);
    layout->pushFloat(2);
    layout->pushFloat(3);
    layout->pushFloat(3);
    layout->pushFloat(3);
    vao = new VertexArrayObject();
    vbo = new VertexBufferObject(vertices, layout->getStride() * numVertices, drawMode);
    vao->addBuffer(*vbo, *layout);
    ibo = nullptr;
    delete layout;
}

Mesh3D::Mesh3D(const float* vertices, const uint64_t verticesSize, const unsigned int* indices, const uint64_t indicesSize, unsigned int drawType, unsigned int drawMode) : numIndices(indicesSize), drawType(drawType) {
    auto layout = new VertexBufferObjectLayout();
    layout->pushFloat(3);
    layout->pushFloat(2);
    layout->pushFloat(3);
    layout->pushFloat(3);
    layout->pushFloat(3);
    vbo = new VertexBufferObject(vertices, layout->getStride() * (verticesSize / 14), drawMode);
    vao = new VertexArrayObject();
    vao->addBuffer(*vbo, *layout);
    ibo = new IndicesBufferObject(indices, numIndices, GL_STATIC_DRAW);
    delete layout;
}

void Mesh3D::changeVertices(const float* vertices, const uint64_t verticesSize, unsigned int offset, unsigned int drawMode){
    vbo->subData(vertices, verticesSize, offset, drawMode);
}

void Mesh3D::changeIndices(const float* indices, const uint64_t indicesSize, unsigned int offset, unsigned int drawMode){
    ibo->subData(indices, indicesSize, offset, drawMode);
}

void Mesh3D::setMaterial(Material& material){
    this->material = material;
    uniformNames = new Uniforms;
}

void Mesh3D::renderMaterialColor(Shader* shader) {
    shader->setUniform3f(uniformNames->diffuse, material.diffuseX, material.diffuseY, material.diffuseZ);
    shader->setUniform3f(uniformNames->specular, material.specularX, material.specularY, material.specularZ);
    shader->setUniform3f(uniformNames->emissive, material.emissiveX, material.emissiveY, material.emissiveZ);
    shader->setUniform1f(uniformNames->shininess, material.shininess);
    vao->bind();
    glDrawElements(drawType, numIndices, GL_UNSIGNED_INT, nullptr);
}

void Mesh3D::renderMaterialTexture(Shader* shader) {
    if(material.diffuseTexture != nullptr) {
        material.diffuseTexture->bind();
    }
    if(material.normalTexture != nullptr) {
        glActiveTexture(GL_TEXTURE1);
        material.normalTexture->bind();
    }
    if(material.specularTexture != nullptr) {
        glActiveTexture(GL_TEXTURE2);
        material.specularTexture->bind();
    }
    if(material.ambientTexture != nullptr) {
        glActiveTexture(GL_TEXTURE3);
        material.ambientTexture->bind();
    }
    if(material.displacementTexture != nullptr) {
        glActiveTexture(GL_TEXTURE4);
        material.displacementTexture->bind();
    }
    glActiveTexture(GL_TEXTURE0);
    vao->bind();
    if(ibo != nullptr)
        glDrawElements(drawType, numIndices, GL_UNSIGNED_INT, nullptr);
    else
        glDrawArrays(drawType, 0, numVertices);
}

void Mesh3D::renderMaterial(Shader* shader) {
    if(material.diffuseTexture != nullptr) {
        material.diffuseTexture->bind();
    }
    if(material.normalTexture != nullptr) {
        glActiveTexture(GL_TEXTURE1);
        material.normalTexture->bind();
    }
    if(material.specularTexture != nullptr) {
        glActiveTexture(GL_TEXTURE2);
        material.specularTexture->bind();
    }
    if(material.ambientTexture != nullptr) {
        glActiveTexture(GL_TEXTURE3);
        material.ambientTexture->bind();
    }
    if(material.displacementTexture != nullptr) {
        glActiveTexture(GL_TEXTURE4);
        material.displacementTexture->bind();
    }
    glActiveTexture(GL_TEXTURE0);
    shader->setUniform3f(uniformNames->diffuse, material.diffuseX, material.diffuseY, material.diffuseZ);
    shader->setUniform3f(uniformNames->specular, material.specularX, material.specularY, material.specularZ);
    shader->setUniform3f(uniformNames->emissive, material.emissiveX, material.emissiveY, material.emissiveZ);
    shader->setUniform1f(uniformNames->shininess, material.shininess);
    vao->bind();
    if(ibo != nullptr)
        glDrawElements(drawType, numIndices, GL_UNSIGNED_INT, nullptr);
    else
        glDrawArrays(drawType, 0, numVertices);
}

void Mesh3D::render() {
    vao->bind();
    if(ibo != nullptr)
        glDrawElements(drawType, numIndices, GL_UNSIGNED_INT, nullptr);
    else
        glDrawArrays(drawType, 0, numVertices);
}

void Mesh3D::draw(){
    if(ibo != nullptr)
        glDrawElements(drawType, numIndices, GL_UNSIGNED_INT, nullptr);
    else
        glDrawArrays(drawType, 0, numVertices);
}

void Mesh3D::bind() {
    vao->bind();
}

void Mesh3D::unbind() {
    VertexArrayObject::unbind();
}

Mesh3D::~Mesh3D() {
    delete vao;
    delete vbo;
    delete ibo;
    delete uniformNames;
}