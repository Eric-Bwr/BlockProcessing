#pragma once

#include <Math/Matrix.h>
#include "PlayerDefines.h"
#include "../Terrain/TerrainDefines.h"

class PlayerCamera {
protected:
    void initCamera(double x, double y, double z, float yaw, float pitch);
public:
    inline double getX(){ return camPos.x; }
    inline double getY(){ return camPos.y; }
    inline double getZ(){ return camPos.z; }
    inline double getCameraX(){ return camPos.x; }
    inline double getCameraY(){ return camPos.y; }
    inline double getCameraZ(){ return camPos.z; }
    void moveMouse(double xPos, double yPos);
    Mat4& getViewMatrix();
    Vec3 camPos, front, up, right;
    Mat4 view;
    Vec3 worldUp;
    bool hasLastPos = false;
    double lastPosX, lastPosY, xOffset, yOffset, yaw, pitch;
    bool shouldMoveForward = false, shouldMoveBackward = false, shouldMoveRight = false, shouldMoveLeft = false, shouldMoveUp = false, shouldMoveDown = false;
private:
    void calculateCamera();
};
