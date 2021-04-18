#pragma once

#include <Math/Matrix.h>
#include "PlayerDefines.h"

class PlayerCamera {
protected:
    static void initCamera(float x, float y, float z, float yaw, float pitch);
public:
    static inline float getX(){ return camPos.x; }
    static inline float getY(){ return camPos.y; }
    static inline float getZ(){ return camPos.z; }
    static void moveMouse(double xPos, double yPos);
    static Mat4f& getViewMatrix();
    static Vec3f camPos, front, up, right;
    static Mat4f view;
    static Vec3f worldUp;
    static bool hasLastPos;
    static float lastPosX, lastPosY, xOffset, yOffset, yaw, pitch;
    static bool shouldMoveForward, shouldMoveBackward, shouldMoveRight, shouldMoveLeft, shouldMoveUp, shouldMoveDown;
private:
    static void calculateCam();
};
