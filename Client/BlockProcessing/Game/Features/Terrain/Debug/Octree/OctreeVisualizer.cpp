#include "OctreeVisualizer.h"
#include "BlockProcessing/Game/Paths.h"
#include <algorithm>

void OctreeVisualizer::init() {
    shader = new Shader(SHADER_OCTREE);
    shader->addUniforms({"projection", "view", "parameters"});
    shader->setUniform2f("parameters", OCTREE_MAX_LEVEL, OCTREE_LENGTH);
    const float vertices[72]{
            0, 0, 0,
            0, CHUNK_SIZE, 0,
            CHUNK_SIZE, 0, 0,
            CHUNK_SIZE, CHUNK_SIZE, 0,
            0, 0, CHUNK_SIZE,
            0, CHUNK_SIZE, CHUNK_SIZE,
            CHUNK_SIZE, 0, CHUNK_SIZE,
            CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE,

            0, CHUNK_SIZE, 0,
            CHUNK_SIZE, CHUNK_SIZE, 0,
            CHUNK_SIZE, CHUNK_SIZE, 0,
            CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE,
            CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE,
            0, CHUNK_SIZE, CHUNK_SIZE,
            0, CHUNK_SIZE, CHUNK_SIZE,
            0, CHUNK_SIZE, 0,

            0, 0, 0,
            CHUNK_SIZE, 0, 0,
            CHUNK_SIZE, 0, 0,
            CHUNK_SIZE, 0, CHUNK_SIZE,
            CHUNK_SIZE, 0, CHUNK_SIZE,
            0, 0, CHUNK_SIZE,
            0, 0, CHUNK_SIZE,
            0, 0, 0
    };
    auto layout = VertexBufferObjectLayout();
    layout.pushFloat(3);
    vao.init();
    vbo.init(vertices, layout.getStride() * 24, GL_STATIC_DRAW);
    vao.addBuffer(vbo, layout);
    glGenBuffers(1, &ssbo);
    stride = 4 * sizeof(float);
}

void OctreeVisualizer::visualizeNode(const std::vector<OctreeNode*>& candidates, bool displayChunks, int closestNodeLevel, Coord minCorner, const Coord& playerCoord, OctreeNode *octreeNode) {
    auto index = octreeNodes.size();
    if(octreeNode->level == OCTREE_MAX_LEVEL) {
        octreeNodes.resize(index + 4);
        octreeNodes[index + 0] = octreeNode->coord.x * CHUNK_SIZE;
        octreeNodes[index + 1] = octreeNode->coord.y * CHUNK_SIZE;
        octreeNodes[index + 2] = octreeNode->coord.z * CHUNK_SIZE;
        octreeNodes[index + 3] = octreeNode->level;
    }else if(octreeNode->level == 0 && displayChunks) {
        const auto& coordinate = [&](OctreeNode* c){
            return Coord::isEqual(c->coord, octreeNode->coord);
        };
        if(std::find_if(candidates.begin(), candidates.end(), coordinate) != candidates.end()){
            octreeNodes.resize(index + 4);
            octreeNodes[index + 0] = octreeNode->coord.x * CHUNK_SIZE;
            octreeNodes[index + 1] = octreeNode->coord.y * CHUNK_SIZE;
            octreeNodes[index + 2] = octreeNode->coord.z * CHUNK_SIZE;
            octreeNodes[index + 3] = octreeNode->level;
        }
    }
    if (octreeNode->level > 0) {
        if(octreeNode->level == closestNodeLevel)
            minCorner = octreeNode->coord;
        for (auto child : octreeNode->children)
            visualizeNode(candidates, displayChunks, closestNodeLevel, minCorner, playerCoord, child);
    }
}

void OctreeVisualizer::visualize(Mat4d& view, bool displayChunks, const std::vector<OctreeNode*>& candidates, int closestNodeLevel, const Coord& playerCoord, OctreeNode *octreeNode) {
    shader->bind();
    vao.bind();
    shader->setUniformMatrix4f("view", view.getBuffer());
    visualizeNode(candidates, displayChunks, closestNodeLevel, {}, playerCoord, octreeNode);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    auto size = octreeNodes.size() / 4;
    if (sizeBefore < size)
        glBufferData(GL_SHADER_STORAGE_BUFFER, size * stride, nullptr, GL_STATIC_DRAW);
    sizeBefore = size;
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, stride * size, octreeNodes.data());
    std::vector<float>().swap(octreeNodes);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    glDrawArraysInstanced(GL_LINES, 0, 24, size);
}

void OctreeVisualizer::setProjection(Mat4f &projection) {
    shader->bind();
    shader->setUniformMatrix4f("projection", projection.getBuffer());
}

OctreeVisualizer::~OctreeVisualizer() {
    VertexArrayObject::unbind();
    VertexBufferObject::unbind();
    delete shader;
}
