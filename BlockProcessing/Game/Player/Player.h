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
    int8_t gameMode = GAMEMODE_CREATIVE;
    int64_t blockX, chunkX, octreeX, lookedBlockX;
    int64_t blockY, chunkY, octreeY, lookedBlockY;
    int64_t blockZ, chunkZ, octreeZ, lookedBlockZ;
    Coord block, chunk, octree, lookedBlock;
    ~Player();
private:
    WorldManager* worldManager;
    PlayerBlockOutline playerBlockOutline;
    void calculateMove(double deltaTime);
    void calculateGravity();
    void castRay();
    unsigned int lookedBlockID, collisionBlockID;
};
