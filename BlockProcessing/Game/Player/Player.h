#pragma once

#include "../Event/EventManager.h"
#include "../Terrain/Util/Coordinate.h"
#include "PlayerCamera.h"
#include "PlayerBlockOutline.h"
#include "../Terrain/World/WorldManager.h"

class Player : public PlayerCamera {
public:
    void init(WorldManager* worldManager, double x, double y, double z, float yaw, float pitch);
    void render(Mat4& view);
    void setProjection(Mat4& projection);
    void update(double deltaTime = 0.0);
    void dig();
    void place();
    bool colliding();
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
    void calculateGravity();
    void castRay();
    void raycast();
    unsigned int lookedBlockID, collisionBlockID;
};
