#include "OctreeVisualizer.h"
#include "BlockProcessing/Game/Paths.h"
#include <algorithm>

void OctreeVisualizer::init() {
    shader = new Shader(SHADER_LINE);
    shader->addUniforms({"projection", "viewModel", "color"});
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
}

void OctreeVisualizer::visualizeNode(const std::vector<OctreeNode*>& candidates, int closestNodeLevel, Coord minCorner, const Coord& playerCoord, OctreeNode *octreeNode) {
    glLineWidth(OCTREE_LINE_WIDTH + octreeNode->level * OCTREE_LINE_WIDTH_AMPLIFIER);
    model.identity();
    model.translate(octreeNode->coord.x * CHUNK_SIZE, octreeNode->coord.y * CHUNK_SIZE, octreeNode->coord.z * CHUNK_SIZE);
    model.scale(octreeNode->scaling);
    shader->setUniformMatrix4f("viewModel", multiplyMatrix(view, model).getBuffer());
    shader->setUniform3f("color", OCTREE_VISUALIZING_COLORS[octreeNode->level * 3 + 0], OCTREE_VISUALIZING_COLORS[octreeNode->level * 3 + 1], OCTREE_VISUALIZING_COLORS[octreeNode->level * 3 + 2]);
    if(octreeNode->level == OCTREE_MAX_LEVEL)
        glDrawArrays(GL_LINES, 0, 24);
    if(octreeNode->level == 0) {
        const auto& coordinate = [&](OctreeNode* c){
            return Coord::isEqual(c->coord, octreeNode->coord);
        };
        if(std::find_if(candidates.begin(), candidates.end(), coordinate) != candidates.end()){
            glDrawArrays(GL_LINES, 0, 24);
        }
    }
    if (octreeNode->level > 0) {
        if(octreeNode->level == closestNodeLevel)
            minCorner = octreeNode->coord;
        for (auto child : octreeNode->children)
            visualizeNode(candidates, closestNodeLevel, minCorner, playerCoord, child);
    }
}

void OctreeVisualizer::visualize(const std::vector<OctreeNode*>& candidates, int closestNodeLevel, Coord& playerCoord, OctreeNode *octreeNode) {
    shader->bind();
    vao.bind();
    visualizeNode(candidates, closestNodeLevel, {}, playerCoord, octreeNode);
}

void OctreeVisualizer::setView(Mat4d &view) {
    this->view = view;
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
