#include "OctreeNode.h"
#include "OctreeLeaf.h"
#include "../World/WorldManager.h"
#include "../../Player/Player.h"

int64_t OctreeNode::called = 0;

OctreeNode::OctreeNode(int level, int scaling, Coord coord) {
    this->coord = coord;
    this->level = level;
    this->scaling = scaling;
    int64_t offsetX, offsetY, offsetZ;
    center = Coord{coord.x + scaling / 2, coord.y + scaling / 2, coord.z + scaling / 2};
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

void OctreeNode::render(Mat4 &view, Shader *shader) {
    if (needsRendering > 0) {
        if (WorldManager::frustum.isInside(frustumCoords[0]) || WorldManager::frustum.isInside(frustumCoords[1])
            || WorldManager::frustum.isInside(frustumCoords[2]) || WorldManager::frustum.isInside(frustumCoords[3])
            || WorldManager::frustum.isInside(frustumCoords[4]) || WorldManager::frustum.isInside(frustumCoords[5])
            || WorldManager::frustum.isInside(frustumCoords[6]) || WorldManager::frustum.isInside(frustumCoords[7])
            || Coord::isEqual(getOctreeFromChunk(coord), Player::octree) || true) {
            if (level == 1) {
                for (auto child : children)
                    ((OctreeLeaf *) child)->render(view, shader);
            } else {
                for (auto child : children)
                    ((OctreeNode *) child)->render(view, shader);
            }
        }
    }
}

void OctreeNode::checkUnload(int64_t tileX, int64_t tileY, int64_t tileZ) {
    if (level != 1) {
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

Coord OctreeNode::closestPossibleChunk(Coord playerCoord) {
    Coord minCorner = coord;
    Coord maxCorner = {minCorner.x + scaling - 1, minCorner.y + scaling - 1, minCorner.z + scaling - 1};
    return {std::min(std::max(playerCoord.x, minCorner.x), maxCorner.x), std::min(std::max(playerCoord.y, minCorner.y), maxCorner.y), std::min(std::max(playerCoord.z, minCorner.z), maxCorner.z)};
}

static void sort(std::vector<Coord> &candidates, const Coord &coord, const Coord &playerChunkCoord) {
    auto it = std::max_element(candidates.begin(), candidates.end(), [&](const Coord &a, const Coord &b) {
        return Coord::distanceSquared(playerChunkCoord, a) - Coord::distanceSquared(playerChunkCoord, b) < 0;
    });
    std::swap(*it, candidates.back());
}

#include "iostream"

void OctreeNode::getClosestChunks(std::vector<Coord> &candidates, int maxCandidates, const Coord &playerChunkCoord) {
    if (childrenLoaded == 8)
        return;
    called++;
    if (level == 1) {
        for (auto child : children) {
            auto leaf = (OctreeLeaf *) child;
            if (!leaf->generating && !leaf->chunk.render) {
                if (candidates.size() >= maxCandidates) {
                    auto currentMaxDist = Coord::distanceSquared(playerChunkCoord, candidates.back());
                    auto distance = Coord::distanceSquared(playerChunkCoord, leaf->coord);
                    if (distance < currentMaxDist) {
                        candidates.pop_back();
                        candidates.push_back(leaf->coord);
                        sort(candidates, leaf->coord, playerChunkCoord);
                    }
                } else {
                    candidates.push_back(leaf->coord);
                    sort(candidates, leaf->coord, playerChunkCoord);
                }
            }
        }
    } else {
        bool visited[8] = {};
        while (true) {
            int child_index = -1;
            int64_t minDistance = INT64_MAX;
            for (int i = 0; i < 8; i++) {
                auto child = (OctreeNode *) children[i];
                int64_t currentMaxDist = candidates.size() < maxCandidates ? INT64_MAX : Coord::distanceSquared(playerChunkCoord, candidates.back());
                auto closestChunk = child->closestPossibleChunk(playerChunkCoord);
                auto d2 = Coord::distanceSquared(closestChunk, playerChunkCoord);
                if (child->childrenLoaded != 8 && d2 < currentMaxDist && d2 < minDistance && !visited[i]) {
                    minDistance = d2;
                    child_index = i;
                }
            }
            if (child_index == -1) {
                break;
            } else {
                visited[child_index] = true;
                ((OctreeNode *) children[child_index])->getClosestChunks(candidates, maxCandidates, playerChunkCoord);
            }
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
        auto soughtChildCoord = Coord{coord.x >= center.x, coord.y >= center.y, coord.z >= center.z};
        for (int i = 0; i < 8; i++) {
            Coord childCoord = {(i & 0b001) != 0, (i & 0b100) != 0, (i & 0b010) != 0};
            if(Coord::isEqual(childCoord, soughtChildCoord)){
                return ((OctreeNode *) children[i])->getLeafNode(coord);
            }
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
        for (auto child : children) {
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
