#include "PlayerCamera.h"

#define radians 0.01745329251994329576923690768489f

Vec3f PlayerCamera::worldUp(0, 1, 0);
bool PlayerCamera::hasLastPos = false;
float PlayerCamera::lastPosX, PlayerCamera::lastPosY, PlayerCamera::xOffset, PlayerCamera::yOffset, PlayerCamera::yaw, PlayerCamera::pitch;
bool PlayerCamera::shouldMoveForward = false, PlayerCamera::shouldMoveBackward = false, PlayerCamera::shouldMoveRight = false, PlayerCamera::shouldMoveLeft = false, PlayerCamera::shouldMoveUp = false, PlayerCamera::shouldMoveDown = false;
Vec3f PlayerCamera::camPos, PlayerCamera::front(0, 0, -1), PlayerCamera::up, PlayerCamera::right;
Mat4f PlayerCamera::view;

void PlayerCamera::initCamera(float x, float y, float z, float cameraYaw, float cameraPitch){
    camPos.x = x;
    camPos.y = y;
    camPos.z = z;
    yaw = cameraYaw;
    pitch = cameraPitch;
    view.identity();
    calculateCam();
}

void PlayerCamera::calculateCam() {
    front.x = std::cos(yaw * radians) * std::cos(pitch * radians);
    front.y = std::sin(pitch * radians);
    front.z = std::sin(yaw * radians) * std::cos(pitch * radians);
    front.norm();
    right = front.cross(worldUp).norm();
    up = (right.cross(front)).norm();
}

void PlayerCamera::moveMouse(double xPos, double yPos) {
    if (!hasLastPos) {
        lastPosX = (float) xPos;
        lastPosY = (float) (yPos * -1);
        hasLastPos = true;
    }

    xOffset = (lastPosX - (float) xPos) * -1;
    yOffset = (float) (yPos * -1) - lastPosY;
    lastPosX = (float) xPos;
    lastPosY = (float) (yPos * -1);

    xOffset *= PLAYER_CAMERA_MOUSE_SENSITIVITY;
    yOffset *= PLAYER_CAMERA_MOUSE_SENSITIVITY;

    yaw += (float) xOffset;
    pitch += (float) yOffset;
    if (pitch >= PLAYER_CAMERA_PITCH_MAX)
        pitch = PLAYER_CAMERA_PITCH_MAX;
    if (pitch <= PLAYER_CAMERA_PITCH_MIN)
        pitch = PLAYER_CAMERA_PITCH_MIN;
    calculateCam();
}

Mat4f &PlayerCamera::getViewMatrix() {
    return view.lookAt(camPos, camPos + front, up);
}