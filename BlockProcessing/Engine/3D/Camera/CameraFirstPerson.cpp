#include "CameraFirstPerson.h"

CameraFirstPerson::CameraFirstPerson(float x, float y, float z, float moveSpeed, float mouseSensitivity, float yaw, float pitch) :
        CameraFirstPerson({x, y, z}, moveSpeed, mouseSensitivity, yaw, pitch) {}

CameraFirstPerson::CameraFirstPerson(const Vector3f& position, float moveSpeed, float mouseSensitivity, float yaw, float pitch)
: worldUp(0, 1, 0), front(0, 0, -1), camPos(position), yaw(yaw), pitch(pitch), moveSpeed(moveSpeed), mouseSensitivity(mouseSensitivity) {
    view.identity();
    calculateCam();
}

void CameraFirstPerson::calculateCam() {
    front.x = std::cos(yaw * radians) * std::cos(pitch * radians);
    front.y = std::sin(pitch * radians);
    front.z = std::sin(yaw * radians) * std::cos(pitch * radians);
    front.normalize();
    right.cross(front, worldUp).normalize();
    up.cross(right, front).normalize();
}

void CameraFirstPerson::update() {
    if(moveForward){
        camPos += front * moveSpeed;
    }
    if(moveBackward){
        camPos -= front * moveSpeed;
    }
    if(moveLeft){
        camPos -= right * moveSpeed;
    }
    if(moveRight){
        camPos += right * moveSpeed;
    }
    if(moveUp){
        camPos.y += moveSpeed;
    }
    if(moveDown){
        camPos.y -= moveSpeed;
    }
}

void CameraFirstPerson::onEvent(es::Event &e) {
    es::EventDispatcher dispatcher(e);
    dispatcher.dispatchMFunc<es::KeyPressedEvent>(BIND_FUNC(CameraFirstPerson::onKeyPressed));
    dispatcher.dispatchMFunc<es::KeyReleasedEvent>(BIND_FUNC(CameraFirstPerson::onKeyReleased));
    dispatcher.dispatchMFunc<es::CursorPositionEvent>(BIND_FUNC(CameraFirstPerson::onMouseMoved));
    dispatcher.dispatchMFunc<es::MouseUpScrollEvent>(BIND_FUNC(CameraFirstPerson::onMouseUpScroll));
    dispatcher.dispatchMFunc<es::MouseDownScrollEvent>(BIND_FUNC(CameraFirstPerson::onMouseDownScroll));
}

bool CameraFirstPerson::onKeyPressed(es::KeyPressedEvent &e) {
    if(attached) {
        if (e.getKey() == keyForward) {
            moveForward = true;
        }
        if (e.getKey() == keyBackward) {
            moveBackward = true;
        }
        if (e.getKey() == keyLeft) {
            moveLeft = true;
        }
        if (e.getKey() == keyRight) {
            moveRight = true;
        }
        if (e.getKey() == keyUp) {
            moveUp = true;
        }
        if (e.getKey() == keyDown) {
            moveDown = true;
        }
    }
    return false;
}

bool CameraFirstPerson::onKeyReleased(es::KeyReleasedEvent &e) {
    if(attached) {
        if (e.getKey() == keyForward)  {
            moveForward = false;
        }
        if (e.getKey() == keyBackward) {
            moveBackward = false;
        }
        if (e.getKey() == keyLeft) {
            moveLeft = false;
        }
        if (e.getKey() == keyRight) {
            moveRight = false;
        }
        if (e.getKey() == keyUp) {
            moveUp = false;
        }
        if (e.getKey() == keyDown) {
            moveDown = false;
        }
    }
    return false;
}

bool CameraFirstPerson::onMouseUpScroll(es::MouseUpScrollEvent& e) {
    if(shouldZoom) {
        if (attached) {
            zoom += zoomSpeed;
            if (zoom <= minZoom)
                zoom = minZoom;
            if (zoom >= maxZoom)
                zoom = maxZoom;
        }
    }
    return false;
}
bool CameraFirstPerson::onMouseDownScroll(es::MouseDownScrollEvent& e) {
    if(shouldZoom) {
        if (attached) {
            zoom -= zoomSpeed;
            if (zoom <= minZoom)
                zoom = minZoom;
            if (zoom >= maxZoom)
                zoom = maxZoom;
        }
    }
    return false;
}

bool CameraFirstPerson::onMouseMoved(es::CursorPositionEvent& e) {
    if(attached) {
        if (!hasLastPos) {
            lastPosX = (float) e.getPosX();
            lastPosY = (float) (e.getPosY() * -1);
            hasLastPos = true;
        }

        xOffset = (lastPosX - (float) e.getPosX()) * -1;
        yOffset = (float) (e.getPosY() * -1) - lastPosY;
        lastPosX = (float) e.getPosX();
        lastPosY = (float) (e.getPosY() * -1);

        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;

        yaw += (float) xOffset;
        pitch += (float) yOffset;
        if (pitch >= maxPitch)
            pitch = maxPitch;
        if (pitch <= minPitch)
            pitch = minPitch;
        calculateCam();
    }
    return false;
}

Matrix4f& CameraFirstPerson::getViewMatrix() {
    return view.lookAt(camPos, camPos + front, up);
}