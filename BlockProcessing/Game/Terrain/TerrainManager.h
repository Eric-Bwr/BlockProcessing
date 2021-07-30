#pragma once

#include "Paths.h"
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
    static void generate(const Coord& playerChunkCoord);
    static void render(Mat4& projectionView, Mat4& view);
    static void setProjection(Mat4& projection);
    static void setLightPosition(double x, double y, double z);
    ~TerrainManager();
public:
    static Shader* shader;
    static Texture* texture;
    static FastNoise* fastNoise;
};
