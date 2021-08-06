#pragma once

#include "BlockProcessing/Game/Event/EventManager.h"
#include "BlockProcessing/Game/Terrain/Util/Coordinate.h"
#include "PlayerCamera.h"
#include "PlayerBlockOutline.h"
#include "BlockProcessing/Game/Terrain/World/WorldManager.h"

class Player : public PlayerCamera {
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
    ~Player();
private:
    WorldManager* worldManager;
    PlayerBlockOutline playerBlockOutline;
    void calculateMove(double deltaTime);
    void castRay();
    void raycast();
    Vec3 convertMovement(double strafe, double forward);
    unsigned int lookedBlockID, collisionBlockID;
    float moveForward = 0.0f;
    float moveStrafe = 0.0f;
    float velocityX = 0.0f;
    float velocityY = 0.0f;
    float velocityZ = 0.0f;
};
