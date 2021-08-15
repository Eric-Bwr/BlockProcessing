#pragma once

#include "BlockProcessing/Game/Game/Terrain/Util/Coordinate.h"
#include "PlayerBlockOutline.h"
#include "PlayerDefines.h"
#include "BlockProcessing/Game/Game/Terrain/World/WorldManager.h"
#include "../../Entity.h"

class Player : public Entity {
public:
    void init(WorldManager* worldManager, double x, double y, double z, float yaw, float pitch);
    void render(Mat4& view);
    void setProjection(Mat4& projection);
    void update(double deltaTime = 0.0);
    void dig();
    void place();
    bool colliding();
    bool isOnGround();
    int8_t gameMode = GAMEMODE_CREATIVE;
    int64_t blockX, chunkX, octreeX, lookedBlockX, prevLookedBlockX;
    int64_t blockY, chunkY, octreeY, lookedBlockY, prevLookedBlockY;
    int64_t blockZ, chunkZ, octreeZ, lookedBlockZ, prevLookedBlockZ;
    Coord block, chunk, octree, lookedBlock;
    void moveMouse(double xPos, double yPos);
    Mat4& getViewMatrix();
    Vec3 front, up, right, worldUp;
    Mat4 view;
    bool hasLastPos = false;
    double lastPosX, lastPosY, xOffset, yOffset;
    ~Player();
private:
    WorldManager* worldManager;
    PlayerBlockOutline playerBlockOutline;
    void calculateMove(double deltaTime);
    void calculateCamera();
    void castRay();
    unsigned int lookedBlockID, collisionBlockID;
};
