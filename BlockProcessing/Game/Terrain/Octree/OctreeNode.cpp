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

void OctreeNode::render() {
    if (true) {
        if (level == 1) {
            for (auto child : children)
                ((OctreeLeaf *) child)->render();
        } else {
            for (auto child : children)
                ((OctreeNode *) child)->render();
        }
    }
}

void OctreeNode::checkUnload(int64_t tileX, int64_t tileY, int64_t tileZ) {
    if(_abs64(tileX - coord.tileX) > CHUNKING_DELETION_RADIUS ||
        _abs64(tileY - coord.tileY) > CHUNKING_DELETION_RADIUS ||
        _abs64(tileZ - coord.tileZ) > CHUNKING_DELETION_RADIUS){
        if (level == 1) {
            for (auto child : children) {
                if (Coord::isEqual(child->coord, coord)) {
                    ChunkManager::unloadChunk(&((OctreeLeaf *) child)->chunk);
                }
            }
        } else {
            //CHECK ALL THIS
            for (auto child : children) {
                if (Coord::isEqual(child->coord, {findLowerValue(coord.tileX, level), findLowerValue(coord.tileY, level), findLowerValue(coord.tileZ, level)}))
                    return ((OctreeNode *) child)->unload();
            }
        }
    }
}

void OctreeNode::unload() {
    if (level == 1) {
        for (auto child : children) {
            if (Coord::isEqual(child->coord, coord)) {
                return (OctreeLeaf *) child;
            }
        }
    } else {
        for (auto child : children) {
            if (Coord::isEqual(child->coord, {findLowerValue(coord.tileX, level), findLowerValue(coord.tileY, level), findLowerValue(coord.tileZ, level)}))
                return ((OctreeNode *) child)->getLeafNode(coord);
        }
    }
}

OctreeLeaf *OctreeNode::getLeafNode(Coord coord) {
    if (level == 1) {
        for (auto child : children) {
            if (Coord::isEqual(child->coord, coord)) {
                return (OctreeLeaf *) child;
            }
        }
    } else {
        for (auto child : children) {
            if (Coord::isEqual(child->coord, {findLowerValue(coord.tileX, level), findLowerValue(coord.tileY, level), findLowerValue(coord.tileZ, level)}))
                return ((OctreeNode *) child)->getLeafNode(coord);
        }
    }
    return nullptr;
}

OctreeNode::~OctreeNode() {
    if (level == 1) {
        for (auto child : children)
            delete (OctreeLeaf *) child;
    } else {
        for (auto child : children)
            delete (OctreeNode *) child;
    }
}
