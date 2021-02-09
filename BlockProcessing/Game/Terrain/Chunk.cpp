#include <iostream>
#include "Chunk.h"

Chunk::Chunk(CubeMesher* cubeMesher, FastNoise* fastNoise) : cubeMesher(cubeMesher), fastNoise(fastNoise) {
    vao = new VertexArrayObject();
    vbo = new VertexBufferObject(nullptr, 0, GL_STREAM_DRAW);
    vao->addBuffer(*vbo, *cubeMesher->layout);
}

void Chunk::generate(uint64_t tileX, uint64_t tileZ) {
    for(int x = 0; x < 1000; x++){
        for(int y = 0; y < 1000; y++){
            float height = fastNoise->GetNoise(x, y);
            height = int(((height + 1) / 2) * 256);
            cubeMesher->addFace(faceData, 2, 0, x, height, y, FACE_BOTTOM);
            cubeMesher->addFace(faceData, 2, 0, x, height, y, FACE_TOP);
            cubeMesher->addFace(faceData, 2, 0, x, height, y, FACE_LEFT);
            cubeMesher->addFace(faceData, 2, 0, x, height, y, FACE_RIGHT);
            cubeMesher->addFace(faceData, 2, 0, x, height, y, FACE_FRONT);
            cubeMesher->addFace(faceData, 2, 0, x, height, y, FACE_BACK);
            cubeMesher->addFace(faceData, 1, 0, x, height - 1, y, FACE_LEFT);
            cubeMesher->addFace(faceData, 1, 0, x, height - 1, y, FACE_RIGHT);
            cubeMesher->addFace(faceData, 1, 0, x, height - 1, y, FACE_FRONT);
            cubeMesher->addFace(faceData, 1, 0, x, height - 1, y, FACE_BACK);
        }
    }
    faceDataSize = faceData.size();
    vertexCount = faceDataSize / 8;
    vbo->bind();
    vbo->subData(faceData.data(), cubeMesher->layout->getStride() * vertexCount, 0, GL_STREAM_DRAW);
}



void Chunk::render() {
    vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

Chunk::~Chunk() {
    faceData.clear();
    delete vao;
    delete vbo;
}
