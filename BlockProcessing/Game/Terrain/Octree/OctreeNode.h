#pragma once

#include <vector>
#include <cstdint>
#include <Math/Matrix.h>
#include <Shader.h>
#include "../TerrainDefines.h"
#include "OctreeNodeObject.h"

class OctreeLeaf;

class OctreeNode : public OctreeNodeObject {
public:
    static int64_t called;
    OctreeNode(int level, int scaling, Coord coord);
    void render(Mat4 &view, Shader* shader);
    void checkUnload(int64_t tileX, int64_t tileY, int64_t tileZ);
    void unload();
    Coord closestPossibleChunk(Coord playerChunkCoord);
    void getClosestChunks(std::vector<Coord>& candidates, int maxCandidates, const Coord& playerChunkCoord);
    OctreeLeaf* getLeafNode(Coord coord);
    void updateChildrenLoaded();
    void updateNeedsRendering();
    int childrenLoaded = 0, needsRendering = 0;
    ~OctreeNode();
    Coord frustumCoords[8];
    Coord center;
};