#pragma once

#include <Paths.h>
#include <Shader.h>
#include <Texture.h>
#include <FastNoise.h>
#include <cmath>
#include "Game/Terrain/Chunk/Chunk.h"
#include "Game/Terrain/Cube/CubeManager.h"
#include "Chunk/ChunkManager.h"
#include "World/WorldManager.h"

class TerrainManager {
public:
    static void init(int seed, FastNoise::NoiseType noiseType, float frequency, int octaves);
    static void generate(int64_t tileX, int64_t tileY, int64_t tileZ);
    static void render(Mat4f& projectionView, Mat4f& view, float x, float y, float z);
    static void setProjection(Mat4f& projection);
    static void setLightPosition(float x, float y, float z);
    static int64_t getChunkPosition(float coord);
    ~TerrainManager();
public:
    static Mat4f model;
    static Shader* shader;
    static Texture* texture;
    static FastNoise* fastNoise;
};
