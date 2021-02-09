#pragma once

#include "Camera.h"
#include "../EventSystem/Event/InputEvent/CursorPositionEvent.h"
#include "../EventSystem/Event/InputEvent/MouseUpScrollEvent.h"
#include "../EventSystem/Event/InputEvent/MouseDownScrollEvent.h"
#include "../EventSystem/Event/InputEvent/KeyPressedEvent.h"
#include "../EventSystem/Event/InputEvent/KeyReleasedEvent.h"
#include "../Util/Key.h"


namespace gfx {
    class PerspectiveCam : public Camera {
    private:
        glm::vec3 cameraPos = glm::vec3();
        glm::vec3 cameraFront = glm::vec3();
        glm::vec3 cameraUp = glm::vec3();
        float yaw = 0, pitch = 0, lastX = 0, lastY = 0;
        float mouseSensitivity = 0;
        float cameraSpeed = 0;
        float zoom = 0, zoomCheck = 0;
        float aspect = 0;
        glm::vec2 z;
        bool firstMouse = true;
    public:
        PerspectiveCam() = default;
        PerspectiveCam(float fov, glm::vec2 z, glm::vec2 windowSize, glm::vec3 position, glm::vec3 front, glm::vec3 up, float mouseSensitivity, float cameraSpeed);
        void onEvent(es::Event& e) override;
        void update(float deltaTime) override;
    private:
        virtual bool onCursorPositionEvent(es::CursorPositionEvent& e);
        virtual bool onMouseUpScrollEvent(es::MouseUpScrollEvent& e);
        virtual bool onMouseDownScrollEvent(es::MouseDownScrollEvent& e);
        virtual bool onKeyPressedEvent(es::KeyPressedEvent& e);
        virtual bool onKeyReleasedEvent(es::KeyReleasedEvent& e);
    };
}