#pragma once

// Namespace Scene Managing System
#include <glm/vec2.hpp>
#include "../EventSystem/EventSystem.h"

namespace sms {
    class Scene {
        const char* sceneName = nullptr;
        bool pop_scene = false;
        Scene* pushedScene = nullptr;
        bool _pushedNewScene = false;
        glm::vec2 size;
    public:
        ~Scene();
        //scene init
        virtual void init() = 0;

        //scene end
        virtual void close() = 0;

        //Event handler func
        void _onEvent(es::Event& e);
        void _onWindowFramebufferSizeEvent(es::WindowFramebufferSizeEvent&);
        virtual void onEvent(es::Event& e) = 0;

        //Update and render funcs
        virtual bool onUpdate(es::ApplicationUpdateEvent& e) = 0;
        virtual bool onRender(es::ApplicationRenderEvent& e) = 0;

        //general scene funcs
        void setSceneName(const char* sceneName);
        const char* getSceneName();

        void setWindowSize(glm::vec2 windowSize);
        glm::vec2 getWindowSize();

        void pop();
        bool checkValidation();
        void push_pop(Scene* scene);
        Scene* getPushedScene();
        bool pushedNewScene();
    };
}// Namespace Scene Managing System
