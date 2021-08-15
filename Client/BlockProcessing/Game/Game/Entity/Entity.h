#pragma once

#include <Math/Vector.h>

class Entity {
public:
    void initEntity(double x, double y, double z, double yaw, double pitch);
    Vec3 convertMovement(double strafe, double forward, float slipperiness);
    inline double getX(){ return position.x; }
    inline double getY(){ return position.y; }
    inline double getZ(){ return position.z; }
    Vec3 position;
    double yaw, pitch;
    float moveForward = 0.0f;
    float moveStrafe = 0.0f;
    float velocityX = 0.0f;
    float velocityY = 0.0f;
    float velocityZ = 0.0f;
    bool shouldMoveForward = false, shouldMoveBackward = false, shouldMoveRight = false, shouldMoveLeft = false, shouldMoveUp = false, shouldMoveDown = false;
};
