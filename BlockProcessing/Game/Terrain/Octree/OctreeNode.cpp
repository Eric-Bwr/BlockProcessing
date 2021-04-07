#include "OctreeNode.h"
#include "OctreeLeaf.h"
#include "../World/WorldManager.h"

OctreeNode::OctreeNode(int level, int scaling, Coord coord) {
    this->coord = coord;
    int64_t offsetX, offsetY, offsetZ;
    this->level = level;
    this->scaling = scaling;
    if (level == 1) {
        for (int i = 0; i < 8; i++) {
            offsetX = (i & 0b001) != 0;
            offsetY = (i & 0b100) != 0;
            offsetZ = (i & 0b010) != 0;
            children[i] = new OctreeLeaf({coord.tileX + (offsetX * (scaling / 2)), coord.tileY + (offsetY * (scaling / 2)), coord.tileZ + (offsetZ * (scaling / 2))});
        }
    } else {
        for (int i = 0; i < 8; i++) {
            offsetX = (i & 0b001) != 0;
            offsetY = (i & 0b100) != 0;
            offsetZ = (i & 0b010) != 0;
            children[i] = new OctreeNode(level - 1, scaling / 2, {coord.tileX + (offsetX * (scaling / 2)), coord.tileY + (offsetY * (scaling / 2)), coord.tileZ + (offsetZ * (scaling / 2))});
        }
    }
}

void OctreeNode::load() {
    if (level == 1) {
        for (auto child : children)
            ((OctreeLeaf *) child)->load();
    } else {
        for (auto child : children)
            ((OctreeNode *) child)->load();
    }
}

void OctreeNode::render() {
    if (WorldManager::frustum.isInside(coord)) {
        if (level == 1) {
            for (auto child : children)
                ((OctreeLeaf *) child)->load();
        } else {
            for (auto child : children)
                ((OctreeNode *) child)->render();
        }
    }
}

OctreeLeaf *OctreeNode::getLeafNode(Coord coord) {
    if (level == 1) {
        for (auto child : children) {
            if (Coord::isEqual(child->coord, coord))
                return (OctreeLeaf *) child;
        }
    }else{
        for (auto child : children) {
            if (Coord::isEqual(child->coord, coord))
                return (OctreeLeaf *) child;
        }
    }
}

OctreeNode::~OctreeNode() {
    for (auto child : children)
        delete child;
}
