#pragma once

#include <Math/Matrix.h>
#include "PlayerDefines.h"
#include "../Terrain/TerrainDefines.h"

class PlayerCamera {
protected:
    static void initCamera(double x, double y, double z, float yaw, float pitch);
public:
    static inline double getX(){ return camPos.x; }
    static inline double getY(){ return camPos.y; }
    static inline double getZ(){ return camPos.z; }
    static inline double getCameraX(){ return camPos.x; }
    static inline double getCameraY(){ return camPos.y; }
    static inline double getCameraZ(){ return camPos.z; }
    static void moveMouse(double xPos, double yPos);
    static Mat4& getViewMatrix();
    static Vec3 camPos, front, up, right;
    static Mat4 view;
    static Vec3 worldUp;
    static bool hasLastPos;
    static double lastPosX, lastPosY, xOffset, yOffset, yaw, pitch;
    static bool shouldMoveForward, shouldMoveBackward, shouldMoveRight, shouldMoveLeft, shouldMoveUp, shouldMoveDown;
private:
    static void calculateCam();
};
