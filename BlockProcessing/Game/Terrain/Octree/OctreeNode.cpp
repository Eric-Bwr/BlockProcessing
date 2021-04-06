#include "OctreeNode.h"

#define BRB 0
#define BLB 1
#define BRF 2
#define BLF 3
#define TRB 4
#define TLB 5
#define TRF 6
#define TLF 7

#include "iostream"

OctreeNode::OctreeNode(int position, int level, int scaling) {
    this->scaling = scaling;
    this->level = level;
    if (this->level == 0)
        isRootNode = false;
    else {
        for (int i = 0; i < 8; i++) {
            children[i] = new OctreeNode(i, this->level - 1, scaling / 2);
        }
    }
    switch (position) {
        case BRB:
            offsetX = 1;
            offsetY = 0;
            offsetZ = 0;
            break;
        case BLB:
            offsetX = 0;
            offsetY = 0;
            offsetZ = 0;
            break;
        case BRF:
            offsetX = 1;
            offsetY = 0;
            offsetZ = 1;
            break;
        case BLF:
            offsetX = 0;
            offsetY = 0;
            offsetZ = 1;
            break;
        case TRB:
            offsetX = 1;
            offsetY = 1;
            offsetZ = 0;
            break;
        case TLB:
            offsetX = 0;
            offsetY = 1;
            offsetZ = 0;
            break;
        case TRF:
            offsetX = 1;
            offsetY = 1;
            offsetZ = 1;
            break;
        case TLF:
            offsetX = 0;
            offsetY = 1;
            offsetZ = 1;
            break;
        default:
            offsetX = 0;
            offsetY = 0;
            offsetZ = 0;
            break;
    }
}

//MOVE 0.5 TO INIT AS NOT ONLY ONE OCTREE BUT CHUNKING -> OCTREE ----> AKS MR. TOUSSANT
void OctreeNode::updateOctree(int64_t tileX, int64_t tileY, int64_t tileZ) {
    if (isRootNode) {
        for (auto child : children) {
            child->updateOctree(tileX + (offsetX * (scaling / 2)), tileY + (offsetY * (scaling / 2)), tileZ + (offsetZ * (scaling / 2)));
        }
    } else {
        if (chunk != nullptr) {
            if (_abs64(chunk->coord.tileX - (tileX + offsetX)) > CHUNKING_DELETION_RADIUS) {
                if (_abs64(chunk->coord.tileY - (tileY + offsetY)) > CHUNKING_DELETION_RADIUS) {
                    if (_abs64(chunk->coord.tileZ - (tileZ + offsetZ)) > CHUNKING_DELETION_RADIUS) {
                        //DELETE CURRENT CHUNK AND GEN NEW ONE
                    }
                }
            }
        }
    }
    model.identity();
    model.translate(tileX * WORLD_SIZE, tileY * WORLD_SIZE, tileZ * WORLD_SIZE);
    model.scale(scaling);
}

void OctreeNode::renderOctree(Shader *shader) {
    if (isRootNode) {
        for (auto child : children) {
            child->renderOctree(shader);
        }
    } else {
        //RENDER THE FUCKIN THING
    }
}

void OctreeNode::visualize(Shader *visualizeShader) {
    glLineWidth(OCTREE_LINE_WIDTH + level * OCTREE_LINE_WIDTH_AMPLIFIER);
    visualizeShader->setUniformMatrix4f("model", model.getBuffer());
    visualizeShader->setUniform3f("color", OCTREE_VISUALIZING_COLORS[level * 3 + 0], OCTREE_VISUALIZING_COLORS[level * 3 + 1], OCTREE_VISUALIZING_COLORS[level * 3 + 2]);
    glDrawArrays(GL_LINES, 0, 24);
    if (isRootNode) {
        for (auto child : children) {
            child->visualize(visualizeShader);
        }
    }
}

OctreeNode::~OctreeNode() {
    for (auto child : children) {
        delete child;
    }
    delete chunk;
}