#pragma once

#include <GLFW/glfw3.h>
#include <Matrix/Matrix4f.h>
#include <Vector/Vector3f.h>
#include <EventSystem/EventSystem.h>

class CameraFirstPerson {
public:
    CameraFirstPerson(const Vector3f& position, float movementSpeed, float mouseSensitivity, float yaw, float pitch);
    CameraFirstPerson(float x, float y, float z, float movementSpeed, float mouseSensitivity, float yaw, float pitch);
    void onEvent(es::Event& e);
    void update();
    bool onKeyPressed(es::KeyPressedEvent& e);
    bool onKeyReleased(es::KeyReleasedEvent& e);
    bool onMouseMoved(es::CursorPositionEvent& e);
    bool onMouseUpScroll(es::MouseUpScrollEvent& e);
    bool onMouseDownScroll(es::MouseDownScrollEvent& e);
    inline void setZoomingSpeed(float zoomSpeed) { this->zoomSpeed = zoomSpeed; }
    inline void setZooming(bool shouldZoom) { this->shouldZoom = shouldZoom; }
    inline void setAttached(bool attached) { this->attached = attached; }
    inline void setYaw(float yaw) { this->yaw = yaw; calculateCam(); }
    inline void setPitch(float pitch) { this->pitch = pitch; calculateCam(); }
    inline void setZoom(float zoom) { this->zoom = zoom; }
    inline void setMaxPitch(float maxPitch) { this->maxPitch = maxPitch; }
    inline void setMinPitch(float minPitch) { this->minPitch = minPitch; }
    inline void setMovementSpeed(float speed) { moveSpeed = speed; }
    inline void setMouseSensitivity(float sensitivity) { mouseSensitivity = sensitivity; }
    inline void setRightVector(const Vector3f& right) { this->right = right; calculateCam(); }
    inline void setWorldUpVector(const Vector3f& worldUp) { this->worldUp = worldUp; calculateCam(); }
    inline void setViewMatrix(const Matrix4f& view) { this->view = view; }
    inline void setPosition(const Vector3f& position) { this->camPos = position; calculateCam(); }
    inline void setX(const float& x) { this->camPos.x = x; calculateCam(); }
    inline void setY(const float& y) { this->camPos.y = y; calculateCam(); }
    inline void setZ(const float& z) { this->camPos.z = z; calculateCam(); }
    inline void setPosition(float x, float y, float z) { camPos.x = x; camPos.y = y; camPos.z = z; calculateCam(); }
    inline void invertZoom() { this->zoomSpeed *= -1.0f; }
    inline void invertPitch() { this->pitch = -pitch; calculateCam(); }
    inline float getZoom() const { return zoom; }
    inline float getYaw() const { return yaw; }
    inline float getPitch() const { return pitch; }
    inline float getMovementSpeed() const { return moveSpeed; }
    inline float getMouseSensitivity() const { return mouseSensitivity; }
    inline Vector3f& getPosition() { return camPos; }
    inline float& getX() { return camPos.x; }
    inline float& getY() { return camPos.y; }
    inline float& getZ() { return camPos.z; }
    Matrix4f& getViewMatrix();
    int keyForward = GLFW_KEY_W;
    int keyBackward = GLFW_KEY_S;
    int keyLeft = GLFW_KEY_A;
    int keyRight = GLFW_KEY_D;
    int keyUp = GLFW_KEY_SPACE;
    int keyDown = GLFW_KEY_LEFT_SHIFT;
private:
    Matrix4f view;
    Vector3f camPos, front, up, right, worldUp;
    float yaw, pitch, moveSpeed, mouseSensitivity, zoom = 45.0f, zoomSpeed = 1.0f;
    float maxPitch = 89.0f, minPitch = -89.0f, maxZoom = 45.0f, minZoom = 1.0f;
    bool hasLastPos = false, shouldZoom = true, attached = true;
    bool moveForward = false, moveBackward = false, moveRight = false, moveLeft = false, moveUp = false, moveDown = false;
    float lastPosX, lastPosY, xOffset, yOffset;
    void calculateCam();
};