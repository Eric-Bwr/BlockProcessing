#include "Octree.h"
#include "Game/BlockProcessing.h"

OctreeNode::OctreeNode(Frustum* frustum, ChunkManager* chunkManager, int level, int scaling, const Coord& coord, std::vector<OctreeNode> &nodes, std::vector<Chunk> &chunks, int current_nodes_index, int &chunk_index) {
    this->chunkManager = chunkManager;
    this->alreadyConstructed = true;
    this->coord = coord;
    this->level = level;
    this->scaling = scaling;
    this->frustum = frustum;
    int64_t offsetX, offsetY, offsetZ;
    center = Coord{coord.x + scaling / 2, coord.y + scaling / 2, coord.z + scaling / 2};
    if (level == 0) {
        chunk = &chunks[chunk_index++];
        chunk->tileX = coord.x;
        chunk->tileY = coord.y;
        chunk->tileZ = coord.z;
    } else {
        for (int i = 0; i < 8; i++) {
            offsetX = (i & 0b001) != 0;
            offsetY = (i & 0b100) != 0;
            offsetZ = (i & 0b010) != 0;
            Coord childCoord = {coord.x + (offsetX * (scaling / 2)),
                                coord.y + (offsetY * (scaling / 2)),
                                coord.z + (offsetZ * (scaling / 2))};

            int child_nodes_index = 8 * current_nodes_index + i + 1;

            OctreeNode node = OctreeNode(frustum, chunkManager, level - 1, scaling / 2, childCoord, nodes, chunks, child_nodes_index, chunk_index);
            nodes[child_nodes_index] = std::move(node);
            children[i] = &nodes[child_nodes_index];
        }
    }
}

OctreeNode &OctreeNode::operator=(OctreeNode &&other) {
    this->chunkManager = other.chunkManager;
    this->alreadyConstructed = other.alreadyConstructed;
    this->coord = other.coord;
    this->level = other.level;
    this->scaling = other.scaling;
    this->center = other.center;
    this->chunk = other.chunk;
    this->children = other.children;
    this->frustum = other.frustum;

    if(level != 0){
        for(auto child : children){
            child->parent = this;
        }
    }

    other.chunk = nullptr;
    return *this;
}

void OctreeNode::render(Mat4 &view, Shader *shader) {
    if (emptyChildren != ALL_ONES_FLAG) {
        if(frustum->isInside(center, scaling * OCTREE_FRUSTUM_CULLING)) {
            if (level == 0)
                chunkManager->renderChunk(chunk, view, shader);
            else {
                for (auto child : children)
                    child->render(view, shader);
            }
        }
    }
}

Coord OctreeNode::closestContainedChunk(const Coord& playerChunkCoord) const {
    Coord maxCorner = {coord.x + scaling - 1, coord.y + scaling - 1, coord.z + scaling - 1};
    return {std::min(std::max(playerChunkCoord.x, coord.x), maxCorner.x), std::min(std::max(playerChunkCoord.y, coord.y), maxCorner.y), std::min(std::max(playerChunkCoord.z, coord.z), maxCorner.z)};
}

Coord OctreeNode::furthestContainedChunk(const Coord& playerChunkCoord) const{
    Coord maxCorner = {coord.x + scaling - 1, coord.y + scaling - 1, coord.z + scaling - 1};

    Coord furthest = {};
    furthest.x = 2 * playerChunkCoord.x < (maxCorner.x - coord.x) ? maxCorner.x : coord.x;
    furthest.y = 2 * playerChunkCoord.y < (maxCorner.y - coord.y) ? maxCorner.y : coord.y;
    furthest.z = 2 * playerChunkCoord.z < (maxCorner.z - coord.z) ? maxCorner.z : coord.z;

    return furthest;
}

static void sort(std::vector<Coord> &candidates, const Coord &coord, const Coord &playerChunkCoord) {
    auto it = std::max_element(candidates.begin(), candidates.end(), [&](const Coord &a, const Coord &b) {
        return Coord::distanceSquared(playerChunkCoord, a) - Coord::distanceSquared(playerChunkCoord, b) < 0;
    });
    std::swap(*it, candidates.back());
}

void OctreeNode::getClosestUnloadedChunks(std::vector<Coord> &candidates, int maxCandidates, const Coord &playerChunkCoord) {
    if (loadedChildren == OctreeNode::ALL_ONES_FLAG)
        return;
    if (level == 0) {
        if (!chunk->generating && !chunk->loaded) {
            if (candidates.size() >= maxCandidates) {
                auto currentMaxDist = Coord::distanceSquared(playerChunkCoord, candidates.back());
                auto distance = Coord::distanceSquared(playerChunkCoord, coord);
                if (distance < currentMaxDist) {
                    candidates.pop_back();
                    candidates.push_back(coord);
                    sort(candidates, coord, playerChunkCoord);
                }
            } else {
                auto distance = Coord::distanceSquared(playerChunkCoord, coord);
                if(distance < CHUNKING_RADIUS_SQUARED){
                    candidates.push_back(coord);
                    sort(candidates, coord, playerChunkCoord);
                }
            }
        }
    } else {
        bool visited[8] = {};
        while (true) {
            int childIndex = -1;
            int64_t minDistance = INT64_MAX;
            for (int i = 0; i < 8; i++) {
                auto child = children[i];
                int64_t currentMaxDist = candidates.size() < maxCandidates ? INT64_MAX : Coord::distanceSquared(playerChunkCoord, candidates.back());
                auto closestChunk = child->closestContainedChunk(playerChunkCoord);
                auto d2 = Coord::distanceSquared(closestChunk, playerChunkCoord);
                if (child->loadedChildren != OctreeNode::ALL_ONES_FLAG && d2 < currentMaxDist && d2 < minDistance && !visited[i]) {
                    minDistance = d2;
                    childIndex = i;
                }
            }
            if (childIndex == -1) {
                break;
            } else {
                visited[childIndex] = true;
                children[childIndex]->getClosestUnloadedChunks(candidates, maxCandidates, playerChunkCoord);
            }
        }
    }
}

void OctreeNode::deleteFurthestLoadedChunks(const Coord& playerChunkCoord){
    if (emptyChildren == ALL_ONES_FLAG)
        return;
    if (level == 0) {
 		if (!chunk->generating && chunk->loaded) {
            auto distance = Coord::distanceSquared(playerChunkCoord, coord);
            if (distance >= CHUNKING_DELETION_RADIUS_SQUARED) {
                chunkManager->unloadChunk(chunk);
                updateHierarchyLoadedAndEmptyStatus(false, true);
            }
        }
    }else{
        for (int i=0; i<8; i++) {
            auto child = children[i];
            if (child->emptyChildren != ALL_ONES_FLAG){
	            auto furthestChunk = child->furthestContainedChunk(playerChunkCoord);
	            auto d2 = Coord::distanceSquared(furthestChunk, playerChunkCoord);
	            if (d2 >= CHUNKING_DELETION_RADIUS_SQUARED) {
					child->deleteFurthestLoadedChunks(playerChunkCoord);
	            }
            }
        }
    }
}

OctreeNode* OctreeNode::getLeaf(const Coord &inCoord) {
    if (level == 0)
        return this;
    else {
        auto soughtChildCoord = Coord{inCoord.x >= center.x, inCoord.y >= center.y, inCoord.z >= center.z};
        for (int i = 0; i < 8; i++) {
            Coord childCoord = {(i & 0b001) != 0, (i & 0b100) != 0, (i & 0b010) != 0};
            if (Coord::isEqual(childCoord, soughtChildCoord)) {
                return children[i]->getLeaf(inCoord);
            }
        }
    }
    return nullptr;
}

void OctreeNode::updateHierarchyLoadedAndEmptyStatus(bool isChunkLoaded, bool isChunkEmpty){
    OctreeNode* node = this;
    node->loadedChildren = !isChunkLoaded ? 0b00000000 : OctreeNode::ALL_ONES_FLAG;
    node->emptyChildren = !isChunkEmpty ? 0b00000000 : OctreeNode::ALL_ONES_FLAG;
    while(node->parent != nullptr){
        OctreeNode* parent = node->parent;
        int index = 0;
        while(parent->children[index] != node){
            index++;
        }

        const auto& setBit = [](int& number, int n, int value){
            number ^= (-value ^ number) & (1UL << n);
        };

        setBit(parent->loadedChildren, index, node->loadedChildren==OctreeNode::ALL_ONES_FLAG);
        setBit(parent->emptyChildren, index, node->emptyChildren==OctreeNode::ALL_ONES_FLAG);
        node = parent;
    }
}

OctreeNode::~OctreeNode() = default;