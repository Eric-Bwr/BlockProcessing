#pragma once

#include <cmath>
#include <thread>
#include <FastNoise.h>
#include <unordered_map>
#include <unordered_set>
#include "../Chunk/ChunkManager.h"
#include "../Block/BlockManager.h"

struct Hash {
    int operator()(const Coord &coord) const {
        return ((coord.tileX * 31 + coord.tileY) * 31 + coord.tileZ) * 31;
    }
};

struct Compare {
    bool operator()(const Coord &coord1, const Coord& coord2) const {
        return coord1.tileX == coord2.tileX && coord1.tileY == coord2.tileY && coord1.tileZ == coord2.tileZ;
    }
};

class WorldManager {
public:
    static void init();
    static void generate(int64_t tileX, int64_t tileY, int64_t tileZ);
    static void generateChunkData(Chunk* chunk);
    static void getChunkBlock(ChunkBlock* chunkBlock, int x, int y, int z);
    void render();
    ~WorldManager();
public:
    static FastNoise* fastNoise;
    static std::unordered_map<Coord, Chunk*, Hash, Compare> chunks;
    static std::unordered_set<Coord, Hash, Compare> chunksGenerating;
    static std::vector<Coord> chunksCandidatesForGenerating;
};
