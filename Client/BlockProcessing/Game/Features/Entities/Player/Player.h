#pragma once

#include <Math/Matrix.h>
#include "BlockProcessing/Game/Features/Terrain/Util/Coordinate.h"
#include "PlayerDefines.h"
#include "BlockProcessing/Game/Features/Terrain/World/WorldManager.h"
#include "BlockProcessing/Game/Engine/Entity/Entity.h"

class Player : public Entity {
public:
    void init(WorldManager* worldManager, double x, double y, double z, float yaw, float pitch);
    void render(Mat4f& view);
    void setProjection(Mat4f& projection);
    void update(double deltaTime = 0.0);
    void dig();
    void place();
    bool colliding();
    bool isOnGround();
    int64_t blockX, chunkX, octreeX;
    int64_t blockY, chunkY, octreeY;
    int64_t blockZ, chunkZ, octreeZ;
    int64_t prevLookedBlockX, prevLookedBlockY, prevLookedBlockZ;
    int64_t lookedBlockX, lookedBlockY, lookedBlockZ;
    Coord block, chunk, octree, lookedBlock;
    void moveMouse(double xPos, double yPos);
    Mat4f getViewMatrix();
    Vec3f front, up, right, worldUp;
    Mat4f view;
    bool hasLastPos = false;
    double lastPosX, lastPosY, xOffset, yOffset;
    float speed = 500.0f;
    ~Player();
private:
    WorldManager* worldManager;
    void calculateMove(double deltaTime);
    void calculateCamera();
    void castRay();
};
