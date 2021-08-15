#pragma once

#include "BlockProcessing/Game/Paths.h"
#include <Shader.h>
#include <Texture.h>
#include <FastNoise.h>
#include "Chunk/Chunk.h"
#include "Cube/CubeManager.h"
#include "Chunk/ChunkManager.h"
#include "World/WorldManager.h"

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
    FastNoise* fastNoise2;
    BlockManager* blockManager;
    WorldManager* worldManager;
};
