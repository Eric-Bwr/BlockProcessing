// COPYRIGHT BY K.E
// GERMANY 21.01.2020

#pragma once
#define SDL_MAIN_HANDLED
#include "APISelector.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <include/SDL2/SDL.h>
#include "WindowSettings.h"
#include "../EventSystem/EventSystem.h"
#include "../CallbackSystem/Callback.h"
#include "../Math/Vector.h"

namespace ws {
    class Window {
    private:
        //window
        GLFWwindow* glfwWindow = nullptr;
        SDL_Window* sdlWindow = nullptr;
        SDL_GLContext sdlContext;
        WindowSettings settings;
        bool closed = false;
        bool glew = false;
        cs::BasicCallback<void, es::Event&> eventCall;
        float r = 0.1, g = 0.1, b = 0.1;
        void setGLFWFuncs();

        bool throwError = false;
    public:
        Window();
        Window(int width, int height, const char* title);

        void build(int width, int height, const char* title, bool throwError);

        template<typename F>
        void build(int width, int height, const char* title, bool throwError, const F &func) {
            build(width, height, title, throwError);
            bindEventMFunc(func);
        }

        void bindEventFunc(void (*onEvent)(es::Event &));

        template<typename F>
        void bindEventMFunc(const F &func) {
            if (settings.api == API::GLFW) {
                glfwSetWindowUserPointer(this->glfwWindow, this);
                setGLFWFuncs();
            }
            eventCall.bind(func);
        }

        void swapBuffer(void (*func)());
        void beginSwap();
        void endSwap();
        void setClearColor(float r, float g, float b);

        void setErrorThrow(bool value);

        int getWidth();
        int getHeight();

        WindowSettings getWindowSettings();
        uint8_t getWindowID();
        API getWindowAPI();
        void close();
        bool isWindowOpen();

        cs::BasicCallback<void, es::Event&>& getEventCallback();

        static void pollEvents();
        static bool areWindowsOpen();
        static void closeAll();
    };
}