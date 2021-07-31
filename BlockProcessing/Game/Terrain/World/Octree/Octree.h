#pragma once

#include <cassert>
#include <array>
#include <vector>
#include <cstdint>
#include <atomic>
#include <Math/Matrix.h>
#include <Shader.h>
#include "Game/Terrain/TerrainDefines.h"
#include "Game/Terrain/Util/Coordinate.h"
#include "Game/Terrain/Chunk/Chunk.h"

class ChunkManager;

class OctreeNode {
    static const int ALL_ONES_FLAG = 0b11111111;
public:
    OctreeNode();
    OctreeNode(ChunkManager* chunkManager, int level, int scaling, Coord coord, std::vector<OctreeNode>& nodes, std::vector<Chunk>& chunks, int current_nodes_index, int& chunk_index);
    ~OctreeNode();
    OctreeNode& operator=(OctreeNode&& other);
    void render(Mat4 &view, Shader* shader);
    Coord closestContainedChunk(const Coord& playerChunkCoord) const;
    Coord furthestContainedChunk(const Coord& playerChunkCoord) const;
    void getClosestUnloadedChunks(std::vector<Coord>& candidates, int maxCandidates, const Coord& playerChunkCoord);
    void deleteFurthestLoadedChunks(const Coord& playerChunkCoord);
    OctreeNode* getLeaf(const Coord& coord);
    Chunk* chunk = nullptr;
    void updateHierarchyLoadedAndEmptyStatus(bool isChunkLoaded, bool isChunkEmpty);
    bool alreadyConstructed = false;
    //A bitfield of the form 0b11111111 when all children nodes are loaded
    int loadedChildren = 0;
    //A bitfield of the form 0b11111111 if all children contain at least one chunk with non air blocks
    int emptyChildren = ALL_ONES_FLAG;
    Coord center;
    int level, scaling;
    Coord coord;
    std::array<OctreeNode*, 8> children;
    OctreeNode* parent = nullptr;
    ChunkManager* chunkManager;
};
#include "iostream"
class Octree{
public:
    Octree(ChunkManager* chunkManager, const Coord& coord) :
    nodes(((1 << 3*(OCTREE_MAX_LEVEL+1)) - 1) / 7),
    chunks(1 << (3*OCTREE_MAX_LEVEL)) {
        // sum of q^n from n=0 to n=N is (q^(N+1) - 1) / (q-1)
        // 1  +
        // 8  +
        // 64 +
        // ...
        // 8^max_level
        // gives (8^(max_level+1) - 1) / (8 - 1) nodes
        // 8^(max_level+1) = (2^3)^(max_level+1) = 2^(3*(max_level+1))
        int chunk_index = 0;
        auto node = OctreeNode(chunkManager, OCTREE_MAX_LEVEL, OCTREE_LENGTH, coord, nodes, chunks, 0, chunk_index);//build the root node
        nodes[0] = std::move(node);
        assert(chunk_index == chunks.size());
        //std::cout <<chunks.size() * sizeof(Chunk) <<"\n";
        //std::cout <<nodes.size() * sizeof(OctreeNode) <<"\n";
    }
    void printCurrentMemoryUsageInfo(int64_t& totalEmptyChunks, int64_t& totalChunks);
    OctreeNode& getRoot(){
        return nodes[0];
    }
private:
    //There are (8^(max_level+1) - 1) / 7 elements
    std::vector<OctreeNode> nodes;
    std::vector<Chunk> chunks;//The chunks of all the leaf nodes
};