#include <algorithm>
#include "OctreeVisualizer.h"
#include "Paths.h"

static Shader *shader;
static VertexArrayObject *vao;
static VertexBufferObject *vbo;
static VertexBufferObjectLayout *layout;
static Mat4 model;

void OctreeVisualizer::init() {
    shader = new Shader(SHADER_LINE);
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

Coord closestPossibleChunk(int scaling, Coord rootNode, Coord playerCoord) {
    Coord minCorner = rootNode;
    Coord maxCorner = {minCorner.x + scaling - 1, minCorner.y + scaling - 1, minCorner.z + scaling - 1};
    return {std::min(std::max(playerCoord.x, minCorner.x), maxCorner.x), std::min(std::max(playerCoord.y, minCorner.y), maxCorner.y), std::min(std::max(playerCoord.z, minCorner.z), maxCorner.z)};
}

static void visualizeNode(const std::vector<Coord>& candidates, int closestNodeLevel, Coord minCorner, const Coord& playerCoord, OctreeNodeObject *octreeNodeObject) {
    glLineWidth(OCTREE_LINE_WIDTH + octreeNodeObject->level * OCTREE_LINE_WIDTH_AMPLIFIER);
    model.identity();
    model.translate(octreeNodeObject->coord.x * WORLD_SIZE, octreeNodeObject->coord.y * WORLD_SIZE, octreeNodeObject->coord.z * WORLD_SIZE);
    model.scale(octreeNodeObject->scaling);
    shader->setUniformMatrix4f("model", model.getBuffer());
    shader->setUniform3f("color", OCTREE_VISUALIZING_COLORS[octreeNodeObject->level * 3 + 0], OCTREE_VISUALIZING_COLORS[octreeNodeObject->level * 3 + 1], OCTREE_VISUALIZING_COLORS[octreeNodeObject->level * 3 + 2]);
    if(octreeNodeObject->level == OCTREE_MAX_LEVEL){
        glDrawArrays(GL_LINES, 0, 24);
    }
    if(octreeNodeObject->level == 0) {
        const auto& evenMoreFucked = [&](const Coord& c){
            return Coord::isEqual(c, octreeNodeObject->coord);
        };
        const auto fucked = std::find_if(candidates.begin(), candidates.end(), evenMoreFucked);
        if(fucked != candidates.end()){
            glDrawArrays(GL_LINES, 0, 24);
        }
        //auto closestChunk = closestPossibleChunk(1 << closestNodeLevel, minCorner, playerCoord);
        //if (Coord::isEqual(octreeNodeObject->coord, closestChunk))
        //    glDrawArrays(GL_LINES, 0, 24);
    }
    if (octreeNodeObject->level > 0) {
        if(octreeNodeObject->level == closestNodeLevel)
            minCorner = octreeNodeObject->coord;
        for (auto child : ((OctreeNode *) octreeNodeObject)->children)
            visualizeNode(candidates, closestNodeLevel, minCorner, playerCoord, child);
    }
}

void OctreeVisualizer::visualize(std::vector<Coord>& candidates, int closestNodeLevel, Coord& playerCoord, OctreeNodeObject *octreeNodeObject) {
    shader->bind();
    vao->bind();
    visualizeNode(candidates, closestNodeLevel, {}, playerCoord, octreeNodeObject);
}

void OctreeVisualizer::setView(Mat4 &view) {
    shader->bind();
    shader->setUniformMatrix4f("view", view.getBuffer());
}

void OctreeVisualizer::setProjection(Mat4 &projection) {
    shader->bind();
    shader->setUniformMatrix4f("projection", projection.getBuffer());
}

OctreeVisualizer::~OctreeVisualizer() {
    VertexArrayObject::unbind();
    VertexBufferObject::unbind();
    delete vao;
    delete vbo;
    delete layout;
    delete shader;
}