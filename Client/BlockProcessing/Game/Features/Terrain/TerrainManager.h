#pragma once

#include "BlockProcessing/Game/Paths.h"
#include "BlockProcessing/Framework/Engine/Logger/Logger.h"
#include <Shader.h>
#include <Texture.h>
#include "BlockProcessing/Game/Features/Terrain/Chunk/Chunk.h"
#include "BlockProcessing/Game/Features/Terrain/Chunk/ChunkManager.h"
#include "BlockProcessing/Game/Features/Terrain/World/WorldManager.h"
#include "BlockProcessing/Game/Features/Terrain/World/AsyncLoader.h"
#include <memory>

class TerrainManager {
public:
    void init(int seed, FastNoise::NoiseType noiseType, float frequency, int octaves);
    void generate(const Coord &playerChunkCoord, const Coord &playerOctreeCoord);
    void render(Mat4d& projectionView, Mat4d& view);
    void setProjection(Mat4f& projection);
    void setLightPosition(double x, double y, double z);
    inline WorldManager* getWorldManager() { return &worldManager; }
    inline BlockManager* getBlockManager() { return &blockManager; }
    ~TerrainManager();
private:
    Shader* shader;
    BlockManager blockManager;
    WorldManager worldManager;
};
