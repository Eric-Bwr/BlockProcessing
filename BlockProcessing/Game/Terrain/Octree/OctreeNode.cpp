#include "OctreeNode.h"
#include "OctreeLeaf.h"
#include "../World/WorldManager.h"
#include "../../Player/Player.h"

#include "iostream"
OctreeNode::OctreeNode(int level, int scaling, Coord coord) {
    this->coord = coord;
    this->level = level;
    this->scaling = scaling;
    int64_t offsetX, offsetY, offsetZ;
    if (level == 1) {
        for (int i = 0; i < 8; i++) {
            offsetX = (i & 0b001) != 0;
            offsetY = (i & 0b100) != 0;
            offsetZ = (i & 0b010) != 0;
            frustumCoords[i] = {coord.x + offsetX * scaling, coord.y + offsetY * scaling, coord.z + offsetZ * scaling};
         //   std::cout << frustumCoords[i].x << " "<< frustumCoords[i].y << " " << frustumCoords[i].z << "\n";
            children[i] = new OctreeLeaf({coord.x + (offsetX * (scaling / 2)), coord.y + (offsetY * (scaling / 2)), coord.z + (offsetZ * (scaling / 2))});
        }
    } else {
        for (int i = 0; i < 8; i++) {
            offsetX = (i & 0b001) != 0;
            offsetY = (i & 0b100) != 0;
            offsetZ = (i & 0b010) != 0;
            frustumCoords[i] = {coord.x + offsetX * scaling, coord.y + offsetY * scaling, coord.z + offsetZ * scaling};
            children[i] = new OctreeNode(level - 1, scaling / 2, {coord.x + (offsetX * (scaling / 2)), coord.y + (offsetY * (scaling / 2)), coord.z + (offsetZ * (scaling / 2))});
        }
    }
    //std::cout << "\n\n\n";
}

void OctreeNode::render() {
    if (needsRendering > 0) {
        if (WorldManager::frustum.isInside(frustumCoords[0]) || WorldManager::frustum.isInside(frustumCoords[1])
            || WorldManager::frustum.isInside(frustumCoords[2]) || WorldManager::frustum.isInside(frustumCoords[3])
            || WorldManager::frustum.isInside(frustumCoords[4]) || WorldManager::frustum.isInside(frustumCoords[5])
            || WorldManager::frustum.isInside(frustumCoords[6]) || WorldManager::frustum.isInside(frustumCoords[7])
            || Coord::isEqual(getOctreeFromChunk(coord), Player::octree)) {
            if (level == 1) {
                for (auto child : children)
                    ((OctreeLeaf *) child)->render();
            } else {
                for (auto child : children)
                    ((OctreeNode *) child)->render();
            }
        }
    }
}

void OctreeNode::checkUnload(int64_t tileX, int64_t tileY, int64_t tileZ) {
    if (level != 1)  {
        for (auto child : children) {
            if (_abs64(tileX - child->coord.x) > CHUNKING_DELETION_RADIUS ||
                _abs64(tileY - child->coord.y) > CHUNKING_DELETION_RADIUS ||
                _abs64(tileZ - child->coord.z) > CHUNKING_DELETION_RADIUS) {
                ((OctreeNode *) child)->unload();
            }
        }
    }
}

void OctreeNode::unload() {
    if (level == 1) {
        for (auto child : children) {
            if (Coord::isEqual(child->coord, coord)) {
                ((OctreeLeaf *) child)->unload();
            }
        }
    } else {
        for (auto child : children) {
            ((OctreeNode *) child)->unload();
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
            if (Coord::isEqual(child->coord, findLowerValue(coord, level)))
                return ((OctreeNode *) child)->getLeafNode(coord);
        }
    }
    return nullptr;
}

void OctreeNode::updateChildrenLoaded() {
    childrenLoaded = 0;
    if (level == 1) {
        for (auto child : children)
            childrenLoaded += ((OctreeLeaf *) child)->chunk.render;
    } else {
        for (auto child : children) {
            ((OctreeNode *) child)->updateChildrenLoaded();
            childrenLoaded += ((OctreeNode *) child)->childrenLoaded == 8;
        }
    }
}

void OctreeNode::updateNeedsRendering() {
    needsRendering = 0;
    if (level == 1) {
        for (auto child : children)
            needsRendering += ((OctreeLeaf *) child)->chunk.faceDataSize > 0;
    } else {
        for (auto child : children){
            ((OctreeNode *) child)->updateNeedsRendering();
            needsRendering += ((OctreeNode *) child)->needsRendering > 0;
        }
    }
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