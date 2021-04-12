#include "ChunkBorder.h"

ChunkBorder::ChunkBorder() {
    layout = new VertexBufferObjectLayout();
    std::vector<float> faceData;
    auto vertices = new float[576]{
            0, 0, 0, 0,
            WORLD_SIZE, 0, 0, 0,
            WORLD_SIZE, 0, 0, 0,
            WORLD_SIZE, 0, WORLD_SIZE, 0,
            WORLD_SIZE, 0, WORLD_SIZE, 0,
            0, 0, WORLD_SIZE, 0,
            0, 0, WORLD_SIZE, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, WORLD_SIZE, 0, 0,
            0, 0, WORLD_SIZE, 0,
            0, WORLD_SIZE, WORLD_SIZE, 0,
            WORLD_SIZE, 0, 0, 0,
            WORLD_SIZE, WORLD_SIZE, 0, 0,
            WORLD_SIZE, 0, WORLD_SIZE, 0,
            WORLD_SIZE, WORLD_SIZE, WORLD_SIZE, 0,
            0, WORLD_SIZE, 0, 0,
            WORLD_SIZE, WORLD_SIZE, 0, 0,
            WORLD_SIZE, WORLD_SIZE, 0, 0,
            WORLD_SIZE, WORLD_SIZE, WORLD_SIZE, 0,
            WORLD_SIZE, WORLD_SIZE, WORLD_SIZE, 0,
            0, WORLD_SIZE, WORLD_SIZE, 0,
            0, WORLD_SIZE, WORLD_SIZE, 0,
            0, WORLD_SIZE, 0, 0,

            0, WORLD_SIZE, 0, 2,
            0, WORLD_SIZE * 2, 0, 2,
            0, WORLD_SIZE, 0, 2,
            -WORLD_SIZE, WORLD_SIZE, 0, 2,
            0, WORLD_SIZE, 0, 2,
            0, WORLD_SIZE, -WORLD_SIZE, 2,
            0, WORLD_SIZE, WORLD_SIZE, 2,
            0, WORLD_SIZE * 2, WORLD_SIZE, 2,
            0, WORLD_SIZE, WORLD_SIZE, 2,
            0, WORLD_SIZE, WORLD_SIZE * 2, 2,
            0, WORLD_SIZE, WORLD_SIZE, 2,
            -WORLD_SIZE, WORLD_SIZE, WORLD_SIZE, 2,
            WORLD_SIZE, WORLD_SIZE, 0, 2,
            WORLD_SIZE, WORLD_SIZE * 2, 0, 2,
            WORLD_SIZE, WORLD_SIZE, 0, 2,
            WORLD_SIZE * 2, WORLD_SIZE, 0, 2,
            WORLD_SIZE, WORLD_SIZE, 0, 2,
            WORLD_SIZE, WORLD_SIZE, -WORLD_SIZE, 2,
            WORLD_SIZE, WORLD_SIZE, WORLD_SIZE, 2,
            WORLD_SIZE, WORLD_SIZE * 2, WORLD_SIZE, 2,
            WORLD_SIZE, WORLD_SIZE, WORLD_SIZE, 2,
            WORLD_SIZE * 2, WORLD_SIZE, WORLD_SIZE, 2,
            WORLD_SIZE, WORLD_SIZE, WORLD_SIZE, 2,
            WORLD_SIZE, WORLD_SIZE, WORLD_SIZE * 2, 2,

            0, 0, 0, 2,
            0, -WORLD_SIZE, 0, 2,
            0, 0, 0, 2,
            -WORLD_SIZE, 0, 0, 2,
            0, 0, 0, 2,
            0, 0, -WORLD_SIZE, 2,
            0, 0, WORLD_SIZE, 2,
            0, -WORLD_SIZE, WORLD_SIZE, 2,
            0, 0, WORLD_SIZE, 2,
            0, 0, WORLD_SIZE * 2, 2,
            0, 0, WORLD_SIZE, 2,
            -WORLD_SIZE, 0, WORLD_SIZE, 2,
            WORLD_SIZE, 0, 0, 2,
            WORLD_SIZE, -WORLD_SIZE, 0, 2,
            WORLD_SIZE, 0, 0, 2,
            WORLD_SIZE * 2, 0, 0, 2,
            WORLD_SIZE, 0, 0, 2,
            WORLD_SIZE, 0, -WORLD_SIZE, 2,
            WORLD_SIZE, 0, WORLD_SIZE, 2,
            WORLD_SIZE, -WORLD_SIZE, WORLD_SIZE, 2,
            WORLD_SIZE, 0, WORLD_SIZE, 2,
            WORLD_SIZE * 2, 0, WORLD_SIZE, 2,
            WORLD_SIZE, 0, WORLD_SIZE, 2,
            WORLD_SIZE, 0, WORLD_SIZE * 2, 2,

            -WORLD_SIZE, -WORLD_SIZE, WORLD_SIZE * 2, 2,
            -WORLD_SIZE, -WORLD_SIZE, -WORLD_SIZE, 2,
            0, -WORLD_SIZE, WORLD_SIZE * 2, 2,
            0, -WORLD_SIZE, -WORLD_SIZE, 2,
            WORLD_SIZE, -WORLD_SIZE, WORLD_SIZE * 2, 2,
            WORLD_SIZE, -WORLD_SIZE, -WORLD_SIZE, 2,
            WORLD_SIZE * 2, -WORLD_SIZE, WORLD_SIZE * 2, 2,
            WORLD_SIZE * 2, -WORLD_SIZE, -WORLD_SIZE, 2,
            WORLD_SIZE * 2, -WORLD_SIZE, -WORLD_SIZE, 2,
            -WORLD_SIZE, -WORLD_SIZE, -WORLD_SIZE, 2,
            WORLD_SIZE * 2, -WORLD_SIZE, WORLD_SIZE * 2, 2,
            -WORLD_SIZE, -WORLD_SIZE, WORLD_SIZE * 2, 2,
            WORLD_SIZE * 2, -WORLD_SIZE, 0, 2,
            -WORLD_SIZE, -WORLD_SIZE, 0, 2,
            WORLD_SIZE * 2, -WORLD_SIZE, WORLD_SIZE, 2,
            -WORLD_SIZE, -WORLD_SIZE, WORLD_SIZE, 2,

            -WORLD_SIZE, 0, WORLD_SIZE * 2, 2,
            -WORLD_SIZE, 0, -WORLD_SIZE, 2,
            WORLD_SIZE * 2, 0, WORLD_SIZE * 2, 2,
            WORLD_SIZE * 2, 0, -WORLD_SIZE, 2,
            WORLD_SIZE * 2, 0, -WORLD_SIZE, 2,
            -WORLD_SIZE, 0, -WORLD_SIZE, 2,
            WORLD_SIZE * 2, 0, WORLD_SIZE * 2, 2,
            -WORLD_SIZE, 0, WORLD_SIZE * 2, 2,

            -WORLD_SIZE, WORLD_SIZE, WORLD_SIZE * 2, 2,
            -WORLD_SIZE, WORLD_SIZE, -WORLD_SIZE, 2,
            WORLD_SIZE * 2, WORLD_SIZE, WORLD_SIZE * 2, 2,
            WORLD_SIZE * 2, WORLD_SIZE, -WORLD_SIZE, 2,
            WORLD_SIZE * 2, WORLD_SIZE, -WORLD_SIZE, 2,
            -WORLD_SIZE, WORLD_SIZE, -WORLD_SIZE, 2,
            WORLD_SIZE * 2, WORLD_SIZE, WORLD_SIZE * 2, 2,
            -WORLD_SIZE, WORLD_SIZE, WORLD_SIZE * 2, 2,

            -WORLD_SIZE, WORLD_SIZE * 2, WORLD_SIZE * 2, 2,
            -WORLD_SIZE, WORLD_SIZE * 2, -WORLD_SIZE, 2,
            0, WORLD_SIZE * 2, WORLD_SIZE * 2, 2,
            0, WORLD_SIZE * 2, -WORLD_SIZE, 2,
            WORLD_SIZE, WORLD_SIZE * 2, WORLD_SIZE * 2, 2,
            WORLD_SIZE, WORLD_SIZE * 2, -WORLD_SIZE, 2,
            WORLD_SIZE * 2, WORLD_SIZE * 2, WORLD_SIZE * 2, 2,
            WORLD_SIZE * 2, WORLD_SIZE * 2, -WORLD_SIZE, 2,
            WORLD_SIZE * 2, WORLD_SIZE * 2, -WORLD_SIZE, 2,
            -WORLD_SIZE, WORLD_SIZE * 2, -WORLD_SIZE, 2,
            WORLD_SIZE * 2, WORLD_SIZE * 2, WORLD_SIZE * 2, 2,
            -WORLD_SIZE, WORLD_SIZE * 2, WORLD_SIZE * 2, 2,
            WORLD_SIZE * 2, WORLD_SIZE * 2, 0, 2,
            -WORLD_SIZE, WORLD_SIZE * 2, 0, 2,
            WORLD_SIZE * 2, WORLD_SIZE * 2, WORLD_SIZE, 2,
            -WORLD_SIZE, WORLD_SIZE * 2, WORLD_SIZE, 2,

            -WORLD_SIZE, -WORLD_SIZE, WORLD_SIZE * 2, 2,
            -WORLD_SIZE, WORLD_SIZE * 2, WORLD_SIZE * 2, 2,
            -WORLD_SIZE, -WORLD_SIZE, -WORLD_SIZE, 2,
            -WORLD_SIZE, WORLD_SIZE * 2, -WORLD_SIZE, 2,
            0, -WORLD_SIZE, WORLD_SIZE * 2, 2,
            0, WORLD_SIZE * 2, WORLD_SIZE * 2, 2,
            0, -WORLD_SIZE, -WORLD_SIZE, 2,
            0, WORLD_SIZE * 2, -WORLD_SIZE, 2,
            WORLD_SIZE, -WORLD_SIZE, WORLD_SIZE * 2, 2,
            WORLD_SIZE, WORLD_SIZE * 2, WORLD_SIZE * 2, 2,
            WORLD_SIZE, -WORLD_SIZE, -WORLD_SIZE, 2,
            WORLD_SIZE, WORLD_SIZE * 2, -WORLD_SIZE, 2,
            WORLD_SIZE * 2, -WORLD_SIZE, WORLD_SIZE * 2, 2,
            WORLD_SIZE * 2, WORLD_SIZE * 2, WORLD_SIZE * 2, 2,
            WORLD_SIZE * 2, -WORLD_SIZE, 0, 2,
            WORLD_SIZE * 2, WORLD_SIZE * 2, 0, 2,
            -WORLD_SIZE, -WORLD_SIZE, 0, 2,
            -WORLD_SIZE, WORLD_SIZE * 2, 0, 2,
            WORLD_SIZE * 2, -WORLD_SIZE, -WORLD_SIZE, 2,
            WORLD_SIZE * 2, WORLD_SIZE * 2, -WORLD_SIZE, 2,
            -WORLD_SIZE, -WORLD_SIZE, WORLD_SIZE, 2,
            -WORLD_SIZE, WORLD_SIZE * 2, WORLD_SIZE, 2,
            WORLD_SIZE * 2, -WORLD_SIZE, WORLD_SIZE, 2,
            WORLD_SIZE * 2, WORLD_SIZE * 2, WORLD_SIZE, 2
    };
    faceData.resize(faceData.size() + 576);
    float *dataPtr = faceData.data() + (faceData.size() - 576);
    memcpy(dataPtr, vertices, 576 * sizeof(float));
    const int definedBorderSize = CHUNK_SIZE * 96;
    float definedBorder[definedBorderSize];
    for (int i = 0; i < definedBorderSize; i += 96) {
        definedBorder[i + 0] = 0;
        definedBorder[i + 1] = 0;
        definedBorder[i + 2] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 3] = 1;
        definedBorder[i + 4] = WORLD_SIZE;
        definedBorder[i + 5] = 0;
        definedBorder[i + 6] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 7] = 1;

        definedBorder[i + 8] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 9] = 0;
        definedBorder[i + 10] = 0;
        definedBorder[i + 11] = 1;
        definedBorder[i + 12] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 13] = 0;
        definedBorder[i + 14] = WORLD_SIZE;
        definedBorder[i + 15] = 1;

        definedBorder[i + 16] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 17] = WORLD_SIZE;
        definedBorder[i + 18] = 0;
        definedBorder[i + 19] = 1;
        definedBorder[i + 20] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 21] = WORLD_SIZE;
        definedBorder[i + 22] = WORLD_SIZE;
        definedBorder[i + 23] = 1;

        definedBorder[i + 24] = 0;
        definedBorder[i + 25] = WORLD_SIZE;
        definedBorder[i + 26] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 27] = 1;
        definedBorder[i + 28] = WORLD_SIZE;
        definedBorder[i + 29] = WORLD_SIZE;
        definedBorder[i + 30] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 31] = 1;

        definedBorder[i + 32] = 0;
        definedBorder[i + 33] = 0;
        definedBorder[i + 34] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 35] = 1;
        definedBorder[i + 36] = 0;
        definedBorder[i + 37] = WORLD_SIZE;
        definedBorder[i + 38] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 39] = 1;

        definedBorder[i + 40] = 0;
        definedBorder[i + 41] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 42] = 0;
        definedBorder[i + 43] = 1;
        definedBorder[i + 44] = 0;
        definedBorder[i + 45] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 46] = WORLD_SIZE;
        definedBorder[i + 47] = 1;

        definedBorder[i + 48] = WORLD_SIZE;
        definedBorder[i + 49] = 0;
        definedBorder[i + 50] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 51] = 1;
        definedBorder[i + 52] = WORLD_SIZE;
        definedBorder[i + 53] = WORLD_SIZE;
        definedBorder[i + 54] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 55] = 1;

        definedBorder[i + 56] = WORLD_SIZE;
        definedBorder[i + 57] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 58] = 0;
        definedBorder[i + 59] = 1;
        definedBorder[i + 60] = WORLD_SIZE;
        definedBorder[i + 61] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 62] = WORLD_SIZE;
        definedBorder[i + 63] = 1;

        definedBorder[i + 64] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 65] = 0;
        definedBorder[i + 66] = WORLD_SIZE;
        definedBorder[i + 67] = 1;
        definedBorder[i + 68] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 69] = WORLD_SIZE;
        definedBorder[i + 70] = WORLD_SIZE;
        definedBorder[i + 71] = 1;

        definedBorder[i + 72] = 0;
        definedBorder[i + 73] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 74] = WORLD_SIZE;
        definedBorder[i + 75] = 1;
        definedBorder[i + 76] = WORLD_SIZE;
        definedBorder[i + 77] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 78] = WORLD_SIZE;
        definedBorder[i + 79] = 1;

        definedBorder[i + 80] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 81] = 0;
        definedBorder[i + 82] = 0;
        definedBorder[i + 83] = 1;
        definedBorder[i + 84] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 85] = WORLD_SIZE;
        definedBorder[i + 86] = 0;
        definedBorder[i + 87] = 1;

        definedBorder[i + 88] = 0;
        definedBorder[i + 89] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 90] = 0;
        definedBorder[i + 91] = 1;
        definedBorder[i + 92] = WORLD_SIZE;
        definedBorder[i + 93] = TERRAIN_SIZE * i / 96;
        definedBorder[i + 94] = 0;
        definedBorder[i + 95] = 1;
    }
    faceData.resize(faceData.size() + definedBorderSize);
    dataPtr = faceData.data() + (faceData.size() - definedBorderSize);
    memcpy(dataPtr, definedBorder, definedBorderSize * sizeof(float));
    layout->pushFloat(3);
    layout->pushFloat(1);
    vertexCount = faceData.size() / 4;
    vao = new VertexArrayObject();
    vbo = new VertexBufferObject(faceData.data(), layout->getStride() * vertexCount, GL_STATIC_DRAW);
    vao->addBuffer(*vbo, *layout);
    faceData.clear();
    delete[] vertices;
}

void ChunkBorder::render() {
    vao->bind();
    glDrawArrays(GL_LINES, 0, vertexCount);
}

ChunkBorder::~ChunkBorder() {
    VertexArrayObject::unbind();
    VertexBufferObject::unbind();
    delete vao;
    delete vbo;
    delete layout;
}
