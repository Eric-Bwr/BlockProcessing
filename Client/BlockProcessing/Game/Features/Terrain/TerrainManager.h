#pragma once

#include "BlockProcessing/Game/Paths.h"
#include "BlockProcessing/Framework/Engine/Logger/Logger.h"
#include <Shader.h>
#include <Texture.h>
#include <FastNoise.h>
#include "BlockProcessing/Game/Features/Terrain/Chunk/Chunk.h"
#include "BlockProcessing/Game/Features/Terrain/Cube/CubeManager.h"
#include "BlockProcessing/Game/Features/Terrain/Chunk/ChunkManager.h"
#include "BlockProcessing/Game/Features/Terrain/World/WorldManager.h"

class TerrainManager {
public:
    void init(CubeManager* cubeManager, BlockManager* blockManager, ChunkManager* chunkManager, WorldManager* worldManager, int seed, FastNoise::NoiseType noiseType, float frequency, int octaves);
    void generate(const Coord& playerChunkCoord);
    void render(Mat4f& projectionView, Mat4f& view);
    void setProjection(Mat4f& projection);
    void setLightPosition(double x, double y, double z);
    ~TerrainManager();
    Shader* shader;
    FastNoise* fastNoise;
    BlockManager* blockManager;
    WorldManager* worldManager;
};
