#pragma once

#include "Camera.h"

namespace gfx {
    class OrthoCamera : public Camera {
    private:
    public:
        OrthoCamera() = default;
        OrthoCamera(glm::vec4 boundrie, glm::vec2 z);

        void onEvent(es::Event& e) override;
        void update(float deltaTime) override;
    };
}