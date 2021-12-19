#pragma once

#include <cassert>
#include <array>
#include <vector>
#include <cstdint>
#include <atomic>
#include <Math/Matrix.h>
#include <Shader.h>
#include "BlockProcessing/Game/Features/Terrain/TerrainDefines.h"
#include "BlockProcessing/Game/Features/Terrain/Util/Coordinate.h"
#include "BlockProcessing/Game/Features/Terrain/Chunk/Chunk.h"
#include "BlockProcessing/Game/Features/Terrain/World/Octree/Frustum/Frustum.h"

class ChunkManager;

class OctreeNode {
    static const int ALL_ONES_FLAG = 0b11111111;
public:
    OctreeNode() = default;
    OctreeNode(int level, int scaling, const Coord& coord, std::vector<OctreeNode>& nodes, std::vector<Chunk>& chunks, int current_nodes_index, int& chunk_index);
    OctreeNode& operator=(OctreeNode&& other);
    void render(Frustum* frustum, ChunkManager* chunkManager);
    Coord closestContainedChunk(const Coord& playerChunkCoord) const;
    Coord furthestContainedChunk(const Coord& playerChunkCoord) const;
    void getClosestUnloadedChunks(std::vector<OctreeNode*>& candidates, int maxCandidates, const Coord& playerChunkCoord);
    void deleteFurthestLoadedChunks(const Coord& playerChunkCoord, ChunkManager* chunkManager);
    OctreeNode* getLeaf(const Coord& coord);
    Chunk* chunk = nullptr;
    void updateHierarchyLoadedAndEmptyStatus(bool isChunkLoaded, bool isChunkEmpty);
    bool alreadyConstructed = false;
    int loadedChildren = 0;
    int emptyChildren = ALL_ONES_FLAG;
    Coord center;
    int level, scaling;
    int chunkingRadiusSquared, chunkingDeletionRadiusSquared;
    Coord coord;
    std::array<OctreeNode*, 8> children;
    OctreeNode* parent = nullptr;
    ~OctreeNode();
};

class Octree{
public:
    Octree(const Coord& coord) : nodes(((1 << 3 * (OCTREE_MAX_LEVEL + 1)) - 1) / 7), chunks(1 << (3*OCTREE_MAX_LEVEL)) {
        int chunk_index = 0;
        auto node = OctreeNode(OCTREE_MAX_LEVEL, OCTREE_LENGTH, coord, nodes, chunks, 0, chunk_index);
        nodes[0] = std::move(node);
    }
    OctreeNode& getRoot(){
        return nodes[0];
    }
    void updateProperties(const int chunkingRadiusSquared, const int chunkingDeletionRadiusSquared){
        for(auto& node : nodes){
            node.chunkingRadiusSquared = chunkingRadiusSquared;
            node.chunkingDeletionRadiusSquared = chunkingDeletionRadiusSquared;
        }
    }
    std::vector<OctreeNode> nodes;
    std::vector<Chunk> chunks;
    ~Octree(){
        nodes.clear();
        chunks.clear();
    }
};
