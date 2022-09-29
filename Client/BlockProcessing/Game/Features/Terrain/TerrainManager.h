#pragma once

#include "BlockProcessing/Game/Paths.h"
#include "BlockProcessing/Framework/Engine/Logger/Logger.h"
#include <OpenGL/Shader.h>
#include <OpenGL/Texture.h>
#include "BlockProcessing/Game/Features/Terrain/Chunk/Chunk.h"
#include "BlockProcessing/Game/Features/Terrain/Chunk/ChunkManager.h"
#include "BlockProcessing/Game/Features/Terrain/World/WorldManager.h"
#include "BlockProcessing/Game/Features/Terrain/World/AsyncLoader.h"
#include "BlockProcessing/Game/Features/Terrain/Debug/Octree/OctreeVisualizer.h"
#include <memory>

class TerrainManager {
public:
    void init(int seed, FastNoise::NoiseType noiseType, float frequency, int octaves);
    void generate(const Coord &playerChunkCoord, const Coord &playerOctreeCoord);
    void visualizeOctree(Mat4d &view, bool displayChunks);
    void render(Mat4d& projectionView, Mat4d& view);
    void setProjection(Mat4f& projection);
    void setLightPosition(double x, double y, double z);
    inline WorldManager* getWorldManager() { return &worldManager; }
    inline BlockManager* getBlockManager() { return &blockManager; }
    ~TerrainManager();
private:
    Shader* shader;
    OctreeVisualizer octreeVisualizer;
    BlockManager blockManager;
    WorldManager worldManager;
};
