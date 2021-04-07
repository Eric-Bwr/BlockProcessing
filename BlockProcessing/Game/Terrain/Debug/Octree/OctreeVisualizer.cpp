#include "OctreeVisualizer.h"
#include "Paths.h"

static Shader* shader;
static VertexArrayObject *vao;
static VertexBufferObject *vbo;
static VertexBufferObjectLayout *layout;
static Mat4f model;

void OctreeVisualizer::init() {
    shader = new Shader(SHADER_OCTREE_VISUALIZE);
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

static void visualizeNode(OctreeNodeObject* octreeNodeObject){
    glLineWidth(OCTREE_LINE_WIDTH + octreeNodeObject->level * OCTREE_LINE_WIDTH_AMPLIFIER);
    model.identity();
    model.translate(octreeNodeObject->coord.tileX * WORLD_SIZE, octreeNodeObject->coord.tileY * WORLD_SIZE, octreeNodeObject->coord.tileZ * WORLD_SIZE);
    model.scale(octreeNodeObject->scaling);
    shader->setUniformMatrix4f("model", model.getBuffer());
    shader->setUniform3f("color", OCTREE_VISUALIZING_COLORS[octreeNodeObject->level * 3 + 0], OCTREE_VISUALIZING_COLORS[octreeNodeObject->level * 3 + 1], OCTREE_VISUALIZING_COLORS[octreeNodeObject->level * 3 + 2]);
    glDrawArrays(GL_LINES, 0, 24);
    if(octreeNodeObject->level > 0){
        for (auto child : ((OctreeNode*)octreeNodeObject)->children)
            visualizeNode(child);
    }
}

void OctreeVisualizer::visualize(Mat4f& view, OctreeNodeObject* octreeNodeObject) {
    shader->bind();
    shader->setUniformMatrix4f("view", view.getBuffer());
    vao->bind();
    visualizeNode(octreeNodeObject);
}

void OctreeVisualizer::setProjection(Mat4f& projection) {
    shader->bind();
    shader->setUniformMatrix4f("projection", projection.getBuffer());
}

OctreeVisualizer::~OctreeVisualizer() {
    delete vao;
    delete vbo;
    delete layout;
    delete shader;
}