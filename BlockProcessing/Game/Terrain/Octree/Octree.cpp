#include "Octree.h"

Octree::Octree(int level, int scaling) {
    for (int i = 0; i < 8; i++) {
        children[i] = new OctreeNode(i, level - 1, scaling / 2);
    }
}

//MOVE 0.5 TO INIT AS NOT ONLY ONE OCTREE BUT CHUNKING -> OCTREE ----> AKS MR. TOUSSANT
void Octree::updateOctree(int64_t tileX, int64_t tileY, int64_t tileZ) {
    for (auto child : children) {
        child->updateOctree(tileX, tileY, tileZ);
    }
}

void Octree::renderOctree(Shader *shader) {
    for (auto child : children) {
        child->renderOctree(shader);
    }
}

void Octree::visualize(Shader *visualizeShader) {
    for (auto child : children) {
        child->visualize(visualizeShader);
    }
}

Octree::~Octree() {
    for (auto child : children) {
        delete child;
    }
}