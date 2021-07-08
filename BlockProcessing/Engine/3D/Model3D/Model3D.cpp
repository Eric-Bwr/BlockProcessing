#include "Model3D.h"

Model3D::Model3D(const char* path, const char* textureFolderPath, unsigned int drawMode) {
    std::string newPath = path;
    newPath.append(".bmf");
    /*
    BinaryBuffer binaryBuffer;
    BinaryBufferError errorCache = binaryBufferReadFromFile(&binaryBuffer, newPath.toString());
    if(errorCache == BBE_CANT_FIND_FILE) {
        if (eventCallback != nullptr) {
            const char *msg = "[Model3D] Failed to locate Model3D";
            msg = ntc::getCombinedCString(msg, newPath.toString());
            es::GraphicalErrorEvent error(SECURITY_HIGHT, 2, msg);
            eventCallback->pushCall(error);
        }
        return;
    }
    if(errorCache == BBE_CANT_ALLOC_MEM) {
        if (eventCallback != nullptr) {
            const char *msg = "[Model3D] Failed to allocate Memory";
            msg = ntc::getCombinedCString(msg, newPath.toString());
            es::GraphicalErrorEvent error(SECURITY_HIGHT, 2, msg);
            eventCallback->pushCall(error);
        }
        return;
    }
    auto numMaterials = binaryBufferPop64(&binaryBuffer);
    std::vector<Material> materials;
    materials.reserve(numMaterials);
    for(uint64_t i = 0; i < numMaterials; i++){
        Material material = {};
        material.diffuseX = binaryBufferPopFloat(&binaryBuffer);
        material.diffuseY = binaryBufferPopFloat(&binaryBuffer);
        material.diffuseZ = binaryBufferPopFloat(&binaryBuffer);
        material.specularX = binaryBufferPopFloat(&binaryBuffer);
        material.specularY = binaryBufferPopFloat(&binaryBuffer);
        material.specularZ = binaryBufferPopFloat(&binaryBuffer);
        material.emissiveX = binaryBufferPopFloat(&binaryBuffer);
        material.emissiveY = binaryBufferPopFloat(&binaryBuffer);
        material.emissiveZ = binaryBufferPopFloat(&binaryBuffer);
        material.shininess = binaryBufferPopFloat(&binaryBuffer);

        auto size = binaryBufferPop64(&binaryBuffer);
        char* diffusePath = (char*)binaryBufferPopStringWithTermination(&binaryBuffer, size);
        size = binaryBufferPop64(&binaryBuffer);
        char* normalPath = (char*)binaryBufferPopStringWithTermination(&binaryBuffer, size);
        size = binaryBufferPop64(&binaryBuffer);
        char* specularPath = (char*)binaryBufferPopStringWithTermination(&binaryBuffer, size);
        size = binaryBufferPop64(&binaryBuffer);
        char* ambientPath = (char*)binaryBufferPopStringWithTermination(&binaryBuffer, size);
        size = binaryBufferPop64(&binaryBuffer);
        char* displacementPath = (char*)binaryBufferPopStringWithTermination(&binaryBuffer, size);

        if(!ntc::isCStringEqual(diffusePath, "NULL")){
            material.diffuseTexture = getTexture(ntc::getCombinedCString(textureFolderPath, diffusePath));
        }
        if(!ntc::isCStringEqual(normalPath, "NULL")){
            material.normalTexture = getTexture(ntc::getCombinedCString(textureFolderPath, normalPath));
        }
        if(!ntc::isCStringEqual(specularPath, "NULL")){
            material.specularTexture = getTexture(ntc::getCombinedCString(textureFolderPath, specularPath));
        }
        if(!ntc::isCStringEqual(ambientPath, "NULL")){
            material.ambientTexture = getTexture(ntc::getCombinedCString(textureFolderPath, ambientPath));
        }
        if(!ntc::isCStringEqual(displacementPath, "NULL")){
            material.displacementTexture = getTexture(ntc::getCombinedCString(textureFolderPath, displacementPath));
        }
        materials.append(material);
        free(diffusePath);
        free(normalPath);
        free(specularPath);
        free(ambientPath);
        free(displacementPath);
    }
    auto numMeshes = binaryBufferPop64(&binaryBuffer);
    for(uint64_t i = 0; i < numMeshes; i++) {
        auto materialIndex = binaryBufferPop64(&binaryBuffer);
        auto numVertices = binaryBufferPop64(&binaryBuffer);
        auto numIndices = binaryBufferPop64(&binaryBuffer);
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        vertices.reserve(numVertices);
        for(uint64_t vi = 0; vi < numVertices; vi++) {
            vertices.emplace_back(binaryBufferPopFloat(&binaryBuffer));
            vertices.emplace_back(binaryBufferPopFloat(&binaryBuffer));
            vertices.emplace_back(binaryBufferPopFloat(&binaryBuffer));
            vertices.emplace_back(binaryBufferPopFloat(&binaryBuffer));
            vertices.emplace_back(binaryBufferPopFloat(&binaryBuffer));
            vertices.emplace_back(binaryBufferPopFloat(&binaryBuffer));
            vertices.emplace_back(binaryBufferPopFloat(&binaryBuffer));
            vertices.emplace_back(binaryBufferPopFloat(&binaryBuffer));
            vertices.emplace_back(binaryBufferPopFloat(&binaryBuffer));
            vertices.emplace_back(binaryBufferPopFloat(&binaryBuffer));
            vertices.emplace_back(binaryBufferPopFloat(&binaryBuffer));
            vertices.emplace_back(binaryBufferPopFloat(&binaryBuffer));
            vertices.emplace_back(binaryBufferPopFloat(&binaryBuffer));
            vertices.emplace_back(binaryBufferPopFloat(&binaryBuffer));
        }
        indices.reserve(numIndices);
        for(uint64_t ii = 0; ii < numIndices; ii++) {
            indices.emplace_back(binaryBufferPop64(&binaryBuffer));
        }
        auto mesh = new Mesh3D(vertices.data(), vertices.size(), indices.data(), numIndices, drawMode);
        mesh->setMaterial(materials.at(materialIndex));
        meshes.emplace_back(mesh);
    }
    binaryBufferDestroy(&binaryBuffer);
     */
}

Model3D::Model3D(const float* vertices, const uint64_t verticesSize, const unsigned int drawMode){
    meshes.emplace_back(new Mesh3D(vertices, verticesSize, drawMode));
}

Model3D::Model3D(const float* vertices, const uint64_t verticesSize, const unsigned int* indices, const uint64_t indicesSize, const unsigned int drawMode){
    meshes.emplace_back(new Mesh3D(vertices, verticesSize, indices, indicesSize, drawMode));
}

Model3D::Model3D(const std::vector<float> &vertices, const unsigned int drawMode) {
    meshes.emplace_back(new Mesh3D(vertices.data(), vertices.size(), drawMode));
}

Model3D::Model3D(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, const unsigned int drawMode){
    meshes.emplace_back(new Mesh3D(vertices.data(), vertices.size(), indices.data(), indices.size(), drawMode));
}

Texture* Model3D::getTexture(const char* path) {
    for(auto texture : loadedTextures){
        if(texture->getPath() == path) {
            return texture;
        }
    }
    auto texture = new Texture(path);
    texture->minLinear();
    texture->magLinear();
    texture->repeat();
    texture->load();
    loadedTextures.emplace_back(texture);
    return texture;
}

void Model3D::setupMaterial(Shader& shader) {
    shader.setUniformLocation(meshes.at(0)->getUniforms()->diffuseTexture, meshes.at(0)->getUniforms()->diffuseTextureLocation);
    shader.setUniformLocation(meshes.at(0)->getUniforms()->normalTexture, meshes.at(0)->getUniforms()->normalTextureLocation);
    shader.setUniformLocation(meshes.at(0)->getUniforms()->specularTexture, meshes.at(0)->getUniforms()->specularTextureLocation);
    shader.setUniformLocation(meshes.at(0)->getUniforms()->ambientTexture, meshes.at(0)->getUniforms()->ambientTextureLocation);
    shader.setUniformLocation(meshes.at(0)->getUniforms()->displacementTexture, meshes.at(0)->getUniforms()->displacementTextureLocation);
}

void Model3D::renderMaterialColor(Shader& shader){
    for(auto mesh : meshes) {
        mesh->renderMaterialColor(&shader);
    }
}

void Model3D::renderMaterialTexture(Shader& shader){
    for(auto mesh : meshes) {
        mesh->renderMaterialTexture(&shader);
    }
}

void Model3D::renderMaterial(Shader& shader){
    for(auto mesh : meshes) {
        mesh->renderMaterial(&shader);
    }
}

void Model3D::render(){
    for(auto mesh : meshes) {
        mesh->render();
    }
}

void Model3D::draw(){
    for(auto mesh : meshes) {
        mesh->draw();
    }
}

void Model3D::bind() {
    for(auto mesh : meshes) {
        mesh->bind();
    }
}

void Model3D::unbind() {
    for(auto mesh : meshes) {
        mesh->unbind();
    }
}

void Model3D::setUniforms(Uniforms &uniforms) {
    for(auto mesh : meshes) {
        mesh->setUniforms(&uniforms);
    }
}

Model3D::~Model3D() {
    meshes.clear();
    loadedTextures.clear();
}