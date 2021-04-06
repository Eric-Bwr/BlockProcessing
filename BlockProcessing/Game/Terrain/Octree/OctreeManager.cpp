#include "OctreeManager.h"
#include "Paths.h"

Shader *OctreeManager::visualizeShader;
Octree *OctreeManager::testTree;

static VertexArrayObject *vao;
static VertexBufferObject *vbo;
static VertexBufferObjectLayout *layout;
static float *vertices;

#include "iostream"

void OctreeManager::init() {
    testTree = new Octree(OCTREE_MAX_LEVEL, pow(2, OCTREE_MAX_LEVEL));
    visualizeShader = new Shader(SHADER_OCTREE_VISUALIZE);
    auto vertices = new float[72]{
            0, 0, 0,
            0, WORLD_SIZE, 0,
            WORLD_SIZE, 0, 0,
            WORLD_SIZE, WORLD_SIZE, 0,
            0, 0, WORLD_SIZE,
            0, WORLD_SIZE, WORLD_SIZE,
            WORLD_SIZE, 0, WORLD_SIZE,
            WORLD_SIZE, WORLD_SIZE, WORLD_SIZE,

            0, WORLD_SIZE, 0,
            WORLD_SIZE, WORLD_SIZE, 0,
            WORLD_SIZE, WORLD_SIZE, 0,
            WORLD_SIZE, WORLD_SIZE, WORLD_SIZE,
            WORLD_SIZE, WORLD_SIZE, WORLD_SIZE,
            0, WORLD_SIZE, WORLD_SIZE,
            0, WORLD_SIZE, WORLD_SIZE,
            0, WORLD_SIZE, 0,

            0, 0, 0,
            WORLD_SIZE, 0, 0,
            WORLD_SIZE, 0, 0,
            WORLD_SIZE, 0, WORLD_SIZE,
            WORLD_SIZE, 0, WORLD_SIZE,
            0, 0, WORLD_SIZE,
            0, 0, WORLD_SIZE,
            0, 0, 0
    };
    layout = new VertexBufferObjectLayout();
    layout->pushFloat(3);
    vao = new VertexArrayObject();
    vbo = new VertexBufferObject(vertices, layout->getStride() * 24, GL_STATIC_DRAW);
    vao->addBuffer(*vbo, *layout);
    delete[] vertices;
}

void OctreeManager::update(int64_t tileX, int64_t tileY, int64_t tileZ) {
    testTree->updateOctree(tileX, tileY, tileZ);
}

void OctreeManager::render(Mat4f &view) {
    // shader->bind();
    // shader->setUniformMatrix4f("model", model.getBuffer());
    // shader->setUniformMatrix4f("view", view.getBuffer());
    // for(auto rootNode : rootNodes){
    //     rootNode->renderOctree(shader);
    // }
}

void OctreeManager::visualize(Mat4f &view) {
    visualizeShader->bind();
    visualizeShader->setUniformMatrix4f("view", view.getBuffer());
    vao->bind();
    testTree->visualize(visualizeShader);
}

void OctreeManager::setProjection(Mat4f &projection) {
    visualizeShader->bind();
    visualizeShader->setUniformMatrix4f("projection", projection.getBuffer());
}

OctreeManager::~OctreeManager() {
    delete testTree;
    delete vao;
    delete vbo;
    delete layout;
    delete visualizeShader;
}
