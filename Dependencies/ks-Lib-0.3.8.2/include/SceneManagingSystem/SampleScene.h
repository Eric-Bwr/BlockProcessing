#pragma once

#include <glm/vec2.hpp>
#include "Scene.h"
#include "../Experimental/Camera.h"
#include "../Experimental/PerspectiveCam.h"

namespace sms {
    class SampleScene : public Scene {
    private:
        gfx::Camera* cam;
    public:
        void init() override {
             setSceneName("SampleScene");
             cam = new gfx::PerspectiveCam(45.0f, {0.1f, 1000.f}, this->getWindowSize(), {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, 0.5, 0.5);
             cam->setKeys({GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_E});
        }

        void close() override {
            delete cam;
        }

        void onEvent(es::Event& e) override {
            cam->onEvent(e);
        }

        bool onUpdate(es::ApplicationUpdateEvent& e) override {
            cam->update(1);

            return false;
        }

        bool onRender(es::ApplicationRenderEvent& e) override {
            return false;
        }
    };
}