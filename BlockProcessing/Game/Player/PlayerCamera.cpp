#include "PlayerCamera.h"

#define radians 0.01745329251994329576923690768489

Vec3 PlayerCamera::worldUp(0, 1, 0);
bool PlayerCamera::hasLastPos = false;
double PlayerCamera::lastPosX, PlayerCamera::lastPosY, PlayerCamera::xOffset, PlayerCamera::yOffset, PlayerCamera::yaw, PlayerCamera::pitch;
bool PlayerCamera::shouldMoveForward = false, PlayerCamera::shouldMoveBackward = false, PlayerCamera::shouldMoveRight = false, PlayerCamera::shouldMoveLeft = false, PlayerCamera::shouldMoveUp = false, PlayerCamera::shouldMoveDown = false;
Vec3 PlayerCamera::camPos, PlayerCamera::front(0, 0, -1), PlayerCamera::up, PlayerCamera::right;
Mat4 PlayerCamera::view;

void PlayerCamera::initCamera(double x, double y, double z, float cameraYaw, float cameraPitch){
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
        lastPosX = xPos;
        lastPosY = (yPos * -1);
        hasLastPos = true;
    }

    xOffset = (lastPosX - xPos) * -1;
    yOffset = (yPos * -1) - lastPosY;
    lastPosX = xPos;
    lastPosY = (yPos * -1);

    xOffset *= PLAYER_CAMERA_MOUSE_SENSITIVITY;
    yOffset *= PLAYER_CAMERA_MOUSE_SENSITIVITY;

    yaw += xOffset;
    pitch += yOffset;
    if (pitch >= PLAYER_CAMERA_PITCH_MAX)
        pitch = PLAYER_CAMERA_PITCH_MAX;
    if (pitch <= PLAYER_CAMERA_PITCH_MIN)
        pitch = PLAYER_CAMERA_PITCH_MIN;
    calculateCam();
}

Mat4 &PlayerCamera::getViewMatrix() {
    return view.lookAt(camPos, camPos + front, up);
}