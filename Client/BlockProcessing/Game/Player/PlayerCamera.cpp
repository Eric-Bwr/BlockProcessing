#include "PlayerCamera.h"

#define radians 0.01745329251994329576923690768489

void PlayerCamera::initCamera(double x, double y, double z, float cameraYaw, float cameraPitch){
    worldUp.y = 1;
    front.z = -1;
    camPos.x = x;
    camPos.y = y;
    camPos.z = z;
    yaw = cameraYaw;
    pitch = cameraPitch;
    view.identity();
    calculateCamera();
}

void PlayerCamera::calculateCamera() {
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
    calculateCamera();
}

Mat4 &PlayerCamera::getViewMatrix() {
    return view.lookAt(camPos, camPos + front, up);
}