#pragma once

#include <Math/Matrix.h>
#include "BlockProcessing/Game/Features/Terrain/Util/Coordinate.h"
#include "PlayerDefines.h"
#include "BlockProcessing/Game/Features/Terrain/World/WorldManager.h"
#include "BlockProcessing/Game/Features/Terrain/Block/BlockManager.h"
#include "BlockProcessing/Game/Engine/Entity/Entity.h"
#include "PlayerBlockOutline.h"

class Player : public Entity {
public:
    void init(WorldManager* worldManager, double x, double y, double z, float yaw, float pitch);
    void render(Mat4d& view);
    void setProjection(Mat4f& projection);
    void update(double deltaTime = 0.0);
    void dig();
    void place();
    bool colliding();
    bool isOnGround();
    int64_t blockX, chunkX, octreeX, lookedBlockX, prevLookedBlockX;
    int64_t blockY, chunkY, octreeY, lookedBlockY, prevLookedBlockY;
    int64_t blockZ, chunkZ, octreeZ, lookedBlockZ, prevLookedBlockZ;
    Coord block, chunk, octree, lookedBlock;
    void moveMouse(double xPos, double yPos);
    Mat4d getViewMatrix();
    Vec3d front, up, right, worldUp;
    Mat4d view;
    bool hasLastPos = false;
    double lastPosX, lastPosY, xOffset, yOffset;
    float speed = 100.0f;
    ~Player();
private:
    WorldManager* worldManager;
    PlayerBlockOutline playerBlockOutline;
    void calculateMove(double deltaTime);
    void calculateCamera();
    void traverseRay();
    int8_t lookedBlockID = BLOCK_AIR, collisionBlockID = BLOCK_AIR;
};
