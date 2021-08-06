#pragma once

#include "BlockProcessing/Paths.h"
#include <Shader.h>
#include <Texture.h>
#include <FastNoise.h>
#include "BlockProcessing/Game/Terrain/Chunk/Chunk.h"
#include "BlockProcessing/Game/Terrain/Cube/CubeManager.h"
#include "BlockProcessing/Game/Terrain/Chunk/ChunkManager.h"
#include "BlockProcessing/Game/Terrain/World/WorldManager.h"

class TerrainManager {
public:
    void init(CubeManager* cubeManager, BlockManager* blockManager, ChunkManager* chunkManager, WorldManager* worldManager, int seed, FastNoise::NoiseType noiseType, float frequency, int octaves);
    void generate(const Coord& playerChunkCoord);
    void render(Mat4& projectionView, Mat4& view);
    void setProjection(Mat4& projection);
    void setLightPosition(double x, double y, double z);
    ~TerrainManager();
    Shader* shader;
    FastNoise* fastNoise;
    BlockManager* blockManager;
    WorldManager* worldManager;
};
