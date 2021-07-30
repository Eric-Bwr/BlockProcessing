#include "ChunkBorderVisualizer.h"
#include "Paths.h"

static Shader* shader;
static Mat4 model;
static VertexArrayObject vao;
static VertexBufferObject vbo;
static int vertexCount;

void ChunkBorderVisualizer::init() {
    shader = new Shader(SHADER_CHUNK_BORDER);
    model.identity();
    model.translate(0, 0, 0);
    std::vector<float> faceData;
    const float vertices[576] = {
            0, 0, 0, 0,
            CHUNK_SIZE, 0, 0, 0,
            CHUNK_SIZE, 0, 0, 0,
            CHUNK_SIZE, 0, CHUNK_SIZE, 0,
            CHUNK_SIZE, 0, CHUNK_SIZE, 0,
            0, 0, CHUNK_SIZE, 0,
            0, 0, CHUNK_SIZE, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, CHUNK_SIZE, 0, 0,
            0, 0, CHUNK_SIZE, 0,
            0, CHUNK_SIZE, CHUNK_SIZE, 0,
            CHUNK_SIZE, 0, 0, 0,
            CHUNK_SIZE, CHUNK_SIZE, 0, 0,
            CHUNK_SIZE, 0, CHUNK_SIZE, 0,
            CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, 0,
            0, CHUNK_SIZE, 0, 0,
            CHUNK_SIZE, CHUNK_SIZE, 0, 0,
            CHUNK_SIZE, CHUNK_SIZE, 0, 0,
            CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, 0,
            CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, 0,
            0, CHUNK_SIZE, CHUNK_SIZE, 0,
            0, CHUNK_SIZE, CHUNK_SIZE, 0,
            0, CHUNK_SIZE, 0, 0,

            0, CHUNK_SIZE, 0, 2,
            0, CHUNK_SIZE * 2, 0, 2,
            0, CHUNK_SIZE, 0, 2,
            -CHUNK_SIZE, CHUNK_SIZE, 0, 2,
            0, CHUNK_SIZE, 0, 2,
            0, CHUNK_SIZE, -CHUNK_SIZE, 2,
            0, CHUNK_SIZE, CHUNK_SIZE, 2,
            0, CHUNK_SIZE * 2, CHUNK_SIZE, 2,
            0, CHUNK_SIZE, CHUNK_SIZE, 2,
            0, CHUNK_SIZE, CHUNK_SIZE * 2, 2,
            0, CHUNK_SIZE, CHUNK_SIZE, 2,
            -CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, 2,
            CHUNK_SIZE, CHUNK_SIZE, 0, 2,
            CHUNK_SIZE, CHUNK_SIZE * 2, 0, 2,
            CHUNK_SIZE, CHUNK_SIZE, 0, 2,
            CHUNK_SIZE * 2, CHUNK_SIZE, 0, 2,
            CHUNK_SIZE, CHUNK_SIZE, 0, 2,
            CHUNK_SIZE, CHUNK_SIZE, -CHUNK_SIZE, 2,
            CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, 2,
            CHUNK_SIZE, CHUNK_SIZE * 2, CHUNK_SIZE, 2,
            CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, 2,
            CHUNK_SIZE * 2, CHUNK_SIZE, CHUNK_SIZE, 2,
            CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, 2,
            CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE * 2, 2,

            0, 0, 0, 2,
            0, -CHUNK_SIZE, 0, 2,
            0, 0, 0, 2,
            -CHUNK_SIZE, 0, 0, 2,
            0, 0, 0, 2,
            0, 0, -CHUNK_SIZE, 2,
            0, 0, CHUNK_SIZE, 2,
            0, -CHUNK_SIZE, CHUNK_SIZE, 2,
            0, 0, CHUNK_SIZE, 2,
            0, 0, CHUNK_SIZE * 2, 2,
            0, 0, CHUNK_SIZE, 2,
            -CHUNK_SIZE, 0, CHUNK_SIZE, 2,
            CHUNK_SIZE, 0, 0, 2,
            CHUNK_SIZE, -CHUNK_SIZE, 0, 2,
            CHUNK_SIZE, 0, 0, 2,
            CHUNK_SIZE * 2, 0, 0, 2,
            CHUNK_SIZE, 0, 0, 2,
            CHUNK_SIZE, 0, -CHUNK_SIZE, 2,
            CHUNK_SIZE, 0, CHUNK_SIZE, 2,
            CHUNK_SIZE, -CHUNK_SIZE, CHUNK_SIZE, 2,
            CHUNK_SIZE, 0, CHUNK_SIZE, 2,
            CHUNK_SIZE * 2, 0, CHUNK_SIZE, 2,
            CHUNK_SIZE, 0, CHUNK_SIZE, 2,
            CHUNK_SIZE, 0, CHUNK_SIZE * 2, 2,

            -CHUNK_SIZE, -CHUNK_SIZE, CHUNK_SIZE * 2, 2,
            -CHUNK_SIZE, -CHUNK_SIZE, -CHUNK_SIZE, 2,
            0, -CHUNK_SIZE, CHUNK_SIZE * 2, 2,
            0, -CHUNK_SIZE, -CHUNK_SIZE, 2,
            CHUNK_SIZE, -CHUNK_SIZE, CHUNK_SIZE * 2, 2,
            CHUNK_SIZE, -CHUNK_SIZE, -CHUNK_SIZE, 2,
            CHUNK_SIZE * 2, -CHUNK_SIZE, CHUNK_SIZE * 2, 2,
            CHUNK_SIZE * 2, -CHUNK_SIZE, -CHUNK_SIZE, 2,
            CHUNK_SIZE * 2, -CHUNK_SIZE, -CHUNK_SIZE, 2,
            -CHUNK_SIZE, -CHUNK_SIZE, -CHUNK_SIZE, 2,
            CHUNK_SIZE * 2, -CHUNK_SIZE, CHUNK_SIZE * 2, 2,
            -CHUNK_SIZE, -CHUNK_SIZE, CHUNK_SIZE * 2, 2,
            CHUNK_SIZE * 2, -CHUNK_SIZE, 0, 2,
            -CHUNK_SIZE, -CHUNK_SIZE, 0, 2,
            CHUNK_SIZE * 2, -CHUNK_SIZE, CHUNK_SIZE, 2,
            -CHUNK_SIZE, -CHUNK_SIZE, CHUNK_SIZE, 2,

            -CHUNK_SIZE, 0, CHUNK_SIZE * 2, 2,
            -CHUNK_SIZE, 0, -CHUNK_SIZE, 2,
            CHUNK_SIZE * 2, 0, CHUNK_SIZE * 2, 2,
            CHUNK_SIZE * 2, 0, -CHUNK_SIZE, 2,
            CHUNK_SIZE * 2, 0, -CHUNK_SIZE, 2,
            -CHUNK_SIZE, 0, -CHUNK_SIZE, 2,
            CHUNK_SIZE * 2, 0, CHUNK_SIZE * 2, 2,
            -CHUNK_SIZE, 0, CHUNK_SIZE * 2, 2,

            -CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE * 2, 2,
            -CHUNK_SIZE, CHUNK_SIZE, -CHUNK_SIZE, 2,
            CHUNK_SIZE * 2, CHUNK_SIZE, CHUNK_SIZE * 2, 2,
            CHUNK_SIZE * 2, CHUNK_SIZE, -CHUNK_SIZE, 2,
            CHUNK_SIZE * 2, CHUNK_SIZE, -CHUNK_SIZE, 2,
            -CHUNK_SIZE, CHUNK_SIZE, -CHUNK_SIZE, 2,
            CHUNK_SIZE * 2, CHUNK_SIZE, CHUNK_SIZE * 2, 2,
            -CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE * 2, 2,

            -CHUNK_SIZE, CHUNK_SIZE * 2, CHUNK_SIZE * 2, 2,
            -CHUNK_SIZE, CHUNK_SIZE * 2, -CHUNK_SIZE, 2,
            0, CHUNK_SIZE * 2, CHUNK_SIZE * 2, 2,
            0, CHUNK_SIZE * 2, -CHUNK_SIZE, 2,
            CHUNK_SIZE, CHUNK_SIZE * 2, CHUNK_SIZE * 2, 2,
            CHUNK_SIZE, CHUNK_SIZE * 2, -CHUNK_SIZE, 2,
            CHUNK_SIZE * 2, CHUNK_SIZE * 2, CHUNK_SIZE * 2, 2,
            CHUNK_SIZE * 2, CHUNK_SIZE * 2, -CHUNK_SIZE, 2,
            CHUNK_SIZE * 2, CHUNK_SIZE * 2, -CHUNK_SIZE, 2,
            -CHUNK_SIZE, CHUNK_SIZE * 2, -CHUNK_SIZE, 2,
            CHUNK_SIZE * 2, CHUNK_SIZE * 2, CHUNK_SIZE * 2, 2,
            -CHUNK_SIZE, CHUNK_SIZE * 2, CHUNK_SIZE * 2, 2,
            CHUNK_SIZE * 2, CHUNK_SIZE * 2, 0, 2,
            -CHUNK_SIZE, CHUNK_SIZE * 2, 0, 2,
            CHUNK_SIZE * 2, CHUNK_SIZE * 2, CHUNK_SIZE, 2,
            -CHUNK_SIZE, CHUNK_SIZE * 2, CHUNK_SIZE, 2,

            -CHUNK_SIZE, -CHUNK_SIZE, CHUNK_SIZE * 2, 2,
            -CHUNK_SIZE, CHUNK_SIZE * 2, CHUNK_SIZE * 2, 2,
            -CHUNK_SIZE, -CHUNK_SIZE, -CHUNK_SIZE, 2,
            -CHUNK_SIZE, CHUNK_SIZE * 2, -CHUNK_SIZE, 2,
            0, -CHUNK_SIZE, CHUNK_SIZE * 2, 2,
            0, CHUNK_SIZE * 2, CHUNK_SIZE * 2, 2,
            0, -CHUNK_SIZE, -CHUNK_SIZE, 2,
            0, CHUNK_SIZE * 2, -CHUNK_SIZE, 2,
            CHUNK_SIZE, -CHUNK_SIZE, CHUNK_SIZE * 2, 2,
            CHUNK_SIZE, CHUNK_SIZE * 2, CHUNK_SIZE * 2, 2,
            CHUNK_SIZE, -CHUNK_SIZE, -CHUNK_SIZE, 2,
            CHUNK_SIZE, CHUNK_SIZE * 2, -CHUNK_SIZE, 2,
            CHUNK_SIZE * 2, -CHUNK_SIZE, CHUNK_SIZE * 2, 2,
            CHUNK_SIZE * 2, CHUNK_SIZE * 2, CHUNK_SIZE * 2, 2,
            CHUNK_SIZE * 2, -CHUNK_SIZE, 0, 2,
            CHUNK_SIZE * 2, CHUNK_SIZE * 2, 0, 2,
            -CHUNK_SIZE, -CHUNK_SIZE, 0, 2,
            -CHUNK_SIZE, CHUNK_SIZE * 2, 0, 2,
            CHUNK_SIZE * 2, -CHUNK_SIZE, -CHUNK_SIZE, 2,
            CHUNK_SIZE * 2, CHUNK_SIZE * 2, -CHUNK_SIZE, 2,
            -CHUNK_SIZE, -CHUNK_SIZE, CHUNK_SIZE, 2,
            -CHUNK_SIZE, CHUNK_SIZE * 2, CHUNK_SIZE, 2,
            CHUNK_SIZE * 2, -CHUNK_SIZE, CHUNK_SIZE, 2,
            CHUNK_SIZE * 2, CHUNK_SIZE * 2, CHUNK_SIZE, 2
    };
    faceData.resize(faceData.size() + 576);
    float *dataPtr = faceData.data() + (faceData.size() - 576);
    memcpy(dataPtr, vertices, 576 * sizeof(float));
    const int definedBorderSize = CHUNK_SIZE * 96;
    float definedBorder[definedBorderSize];
    for (int i = 0; i < definedBorderSize; i += 96) {
        definedBorder[i + 0] = 0;
        definedBorder[i + 1] = 0;
        definedBorder[i + 2] = i / 96;
        definedBorder[i + 3] = 1;
        definedBorder[i + 4] = CHUNK_SIZE;
        definedBorder[i + 5] = 0;
        definedBorder[i + 6] = i / 96;
        definedBorder[i + 7] = 1;

        definedBorder[i + 8] = i / 96;
        definedBorder[i + 9] = 0;
        definedBorder[i + 10] = 0;
        definedBorder[i + 11] = 1;
        definedBorder[i + 12] = i / 96;
        definedBorder[i + 13] = 0;
        definedBorder[i + 14] = CHUNK_SIZE;
        definedBorder[i + 15] = 1;

        definedBorder[i + 16] = i / 96;
        definedBorder[i + 17] = CHUNK_SIZE;
        definedBorder[i + 18] = 0;
        definedBorder[i + 19] = 1;
        definedBorder[i + 20] = i / 96;
        definedBorder[i + 21] = CHUNK_SIZE;
        definedBorder[i + 22] = CHUNK_SIZE;
        definedBorder[i + 23] = 1;

        definedBorder[i + 24] = 0;
        definedBorder[i + 25] = CHUNK_SIZE;
        definedBorder[i + 26] = i / 96;
        definedBorder[i + 27] = 1;
        definedBorder[i + 28] = CHUNK_SIZE;
        definedBorder[i + 29] = CHUNK_SIZE;
        definedBorder[i + 30] = i / 96;
        definedBorder[i + 31] = 1;

        definedBorder[i + 32] = 0;
        definedBorder[i + 33] = 0;
        definedBorder[i + 34] = i / 96;
        definedBorder[i + 35] = 1;
        definedBorder[i + 36] = 0;
        definedBorder[i + 37] = CHUNK_SIZE;
        definedBorder[i + 38] = i / 96;
        definedBorder[i + 39] = 1;

        definedBorder[i + 40] = 0;
        definedBorder[i + 41] = i / 96;
        definedBorder[i + 42] = 0;
        definedBorder[i + 43] = 1;
        definedBorder[i + 44] = 0;
        definedBorder[i + 45] = i / 96;
        definedBorder[i + 46] = CHUNK_SIZE;
        definedBorder[i + 47] = 1;

        definedBorder[i + 48] = CHUNK_SIZE;
        definedBorder[i + 49] = 0;
        definedBorder[i + 50] = i / 96;
        definedBorder[i + 51] = 1;
        definedBorder[i + 52] = CHUNK_SIZE;
        definedBorder[i + 53] = CHUNK_SIZE;
        definedBorder[i + 54] = i / 96;
        definedBorder[i + 55] = 1;

        definedBorder[i + 56] = CHUNK_SIZE;
        definedBorder[i + 57] = i / 96;
        definedBorder[i + 58] = 0;
        definedBorder[i + 59] = 1;
        definedBorder[i + 60] = CHUNK_SIZE;
        definedBorder[i + 61] = i / 96;
        definedBorder[i + 62] = CHUNK_SIZE;
        definedBorder[i + 63] = 1;

        definedBorder[i + 64] = i / 96;
        definedBorder[i + 65] = 0;
        definedBorder[i + 66] = CHUNK_SIZE;
        definedBorder[i + 67] = 1;
        definedBorder[i + 68] = i / 96;
        definedBorder[i + 69] = CHUNK_SIZE;
        definedBorder[i + 70] = CHUNK_SIZE;
        definedBorder[i + 71] = 1;

        definedBorder[i + 72] = 0;
        definedBorder[i + 73] = i / 96;
        definedBorder[i + 74] = CHUNK_SIZE;
        definedBorder[i + 75] = 1;
        definedBorder[i + 76] = CHUNK_SIZE;
        definedBorder[i + 77] = i / 96;
        definedBorder[i + 78] = CHUNK_SIZE;
        definedBorder[i + 79] = 1;

        definedBorder[i + 80] = i / 96;
        definedBorder[i + 81] = 0;
        definedBorder[i + 82] = 0;
        definedBorder[i + 83] = 1;
        definedBorder[i + 84] = i / 96;
        definedBorder[i + 85] = CHUNK_SIZE;
        definedBorder[i + 86] = 0;
        definedBorder[i + 87] = 1;

        definedBorder[i + 88] = 0;
        definedBorder[i + 89] = i / 96;
        definedBorder[i + 90] = 0;
        definedBorder[i + 91] = 1;
        definedBorder[i + 92] = CHUNK_SIZE;
        definedBorder[i + 93] = i / 96;
        definedBorder[i + 94] = 0;
        definedBorder[i + 95] = 1;
    }
    faceData.resize(faceData.size() + definedBorderSize);
    dataPtr = faceData.data() + (faceData.size() - definedBorderSize);
    memcpy(dataPtr, definedBorder, definedBorderSize * sizeof(float));
    auto layout = VertexBufferObjectLayout();
    layout.pushFloat(3);
    layout.pushFloat(1);
    vertexCount = faceData.size() / 4;
    vao.init();
    vbo.init(faceData.data(), layout.getStride() * vertexCount, GL_STATIC_DRAW);
    vao.addBuffer(vbo, layout);
    faceData.clear();
}

void ChunkBorderVisualizer::generate(const Coord& coord) {
    model.m30 = coord.x * CHUNK_SIZE;
    model.m31 = coord.y * CHUNK_SIZE;
    model.m32 = coord.z * CHUNK_SIZE;
}

void ChunkBorderVisualizer::render(Mat4& view) {
    glLineWidth(CHUNK_BORDER_WIDTH);
    shader->bind();
    shader->setUniformMatrix4f("view", view.getBuffer());
    shader->setUniformMatrix4f("model", model.getBuffer());
    vao.bind();
    glDrawArrays(GL_LINES, 0, vertexCount);
}

void ChunkBorderVisualizer::setProjection(Mat4& projection) {
    shader->bind();
    shader->setUniformMatrix4f("projection", projection.getBuffer());
}

ChunkBorderVisualizer::~ChunkBorderVisualizer() {
    delete shader;
}