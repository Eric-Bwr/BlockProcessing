#pragma once

#include <FastNoise.h>
#include <cmath>
#include "Game/Terrain/Chunk/Chunk.h"
#include "Game/Terrain/Cube/CubeManager.h"
#include "Chunk/ChunkManager.h"
#include "World/WorldManager.h"

class TerrainManager {
public:
    explicit TerrainManager(int atlasRows);
    void generate(float x, float y, float z);
    void render(Matrix4f* projectionView) const;
    static int64_t getChunkPosition(float coord);
    int getTerrainHeight(int64_t x, int64_t z) const;
    ~TerrainManager();
public:
    WorldManager* worldManager;
    ChunkManager* chunkManager;
    BlockManager* blockManager;
    CubeManager* cubeManager;
    FastNoise* fastNoise;
};
