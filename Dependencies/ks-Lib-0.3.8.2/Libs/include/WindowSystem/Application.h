#pragma once
#include "Window.h"
#include "../SceneManagingSystem/Scene.h"
#include "../TimeRepresentationSystem/Clock.h"

#define WS_CreateWindow(value) bool createWindow() override { return value; }
#define WS_SetAPI(api) ws::API setAPI() override { return api; }
#define WS_SetWindowWidth(width) int setWindowWidth() override { return width; }
#define WS_SetWindowHeight(height) int setWindowHeight() override { return height; }
#define WS_SetWindowTitle(title) const char* setWindowTitle() override { return (const char*)title; }
#define WS_SetClearColor(r, g, b) std::array<float, 3> setClearColor() override { return {r, g, b}; }
#define WS_LoopApplication(loop) bool loopApplication() override { return loop; }
#define WS_CheckLoopStatement(statement) bool checkLoopStatement() override { return statement; }
#define WS_UseSceneManaging(value) bool useSceneManaging() override { return value; }
#define WS_SetUpdateClock(tickRate) unsigned int setUpdateClock() override { return tickRate; }
#define WS_SetRenderClock(frameRate) unsigned int setRenderClock() override { return frameRate; }
#define WS_SetFiosErrorEventCallback(value) bool setFileStreamErrorEventCallback() override { return value; }
#define WS_SetApplicationErrorEventCallback(value) bool setApplicationErrorEventCallback() override { return value; }
#define WS_SetNetworkErrorEventCallback(value) bool setNetworkErrorEventCallback() override { return value; }
#define WS_SetWindowErrorEventCallback(value) bool setWindowErrorEventCallback() override { return value; }
#define WS_SetAllErrorCallbacks(value) bool setAllErrorCallbacks() override { return value; }
#define WS_HandleFiosErrorEvent(value) bool handleFileStreamErrorEvent() override { return value; }
#define WS_HandleApplicationErrorEvent(value) bool handleApplicationErrorEvent() override { return value; }
#define WS_HandleNetworkErrorEvent(value) bool handleNetworkErrorEvent() override { return value; }
#define WS_HandleWindowErrorEvent(value) bool handleWindowErrorEvent() override { return value; }
#define WS_HandleAllErrorEvents(value) bool handleAllErrorEvents() override { return value; }
#define WS_GetOpenGLErrors(value) bool getOpenGLErrors() override { return value; }

#define WS_SetUpWindow(api, createWindow, width, height, title) WS_SetAPI(api) WS_CreateWindow(createWindow) WS_SetWindowWidth(width) WS_SetWindowHeight(height) WS_SetWindowTitle(title)
#define WS_SetUpLoop(loopApp, loopStatement, sceneManaging, tickRate, frameRate) WS_LoopApplication(loopApp) WS_CheckLoopStatement(loopStatement) WS_UseSceneManaging(sceneManaging) WS_SetUpdateClock(tickRate) WS_SetRenderClock(frameRate)
#define WS_HandleAllErrors WS_SetAllErrorCallbacks(true) WS_HandleAllErrorEvents(true)
#define WS_HandleFiosErrors WS_SetFiosErrorEventCallback(true) WS_HandleFiosErrorEvent(true)
#define WS_HandleApplicationErrors WS_SetApplicationErrorEventCallback(true) WS_HandleApplicationErrorEvent(true)
#define WS_HandleNetworkErrors WS_SetNetworkErrorEventCallback(true) WS_HandleNetworkErrorEvent(true);
#define WS_HandleWindowErrors WS_SetWindowErrorEventCallback(true) WS_HandleWindowErrorEvent(true)

#define WS_GLFW ws::API::GLFW
#define WS_SDL2 ws::API::SDL2

#define WS_SET_APPLICATION_SETTING(settings) settings;
#define WS_APPLICATION_STANDARD_SETTINGS    WS_CreateWindow(true)\
                                            WS_SetAPI(ws::API::GLFW)\
                                            WS_SetWindowWidth(800)\
                                            WS_SetWindowHeight(600)\
                                            WS_SetWindowTitle("MyApplication")\
                                            WS_SetClearColor(0.1f, 0.1f, 0.1f)\
                                            WS_LoopApplication(true)\
                                            WS_CheckLoopStatement(this->window.isWindowOpen())\
                                            WS_UseSceneManaging(true)\
                                            WS_SetUpdateClock(60)\
                                            WS_SetRenderClock(60)\
                                            WS_SetFiosErrorEventCallback(false)\
                                            WS_SetApplicationErrorEventCallback(false)\
                                            WS_SetNetworkErrorEventCallback(false)\
                                            WS_SetWindowErrorEventCallback(false)\
                                            WS_SetAllErrorCallbacks(true)\
                                            WS_HandleFiosErrorEvent(false)\
                                            WS_HandleApplicationErrorEvent(false)\
                                            WS_HandleNetworkErrorEvent(false)\
                                            WS_HandleWindowErrorEvent(false)\
                                            WS_HandleAllErrorEvents(true)\
                                            WS_GetOpenGLErrors(true)

#define WS_CLASS_SETTINGS

namespace ws {
    class Application {
    private:
        void _onEvent(es::Event& e);
        std::vector<sms::Scene*> pushedScenes;
        trs::clock::Clock updateClock, renderClock;

    protected:
        ws::Window window;
    public:
        //Build funcs
        Application() = default;
        virtual ~Application();
        void buildApplication();
        void run();

        //Settings
        virtual bool createWindow();
        virtual ws::API setAPI();
        virtual int setWindowWidth();
        virtual int setWindowHeight();
        virtual const char* setWindowTitle();
        virtual std::array<float, 3> setClearColor();
        virtual bool loopApplication();
        virtual bool checkLoopStatement();
        virtual bool useSceneManaging();
        virtual unsigned int setUpdateClock();
        virtual unsigned int setRenderClock();
        virtual bool setFileStreamErrorEventCallback();
        virtual bool setApplicationErrorEventCallback();
        virtual bool setNetworkErrorEventCallback();
        virtual bool setWindowErrorEventCallback();
        virtual bool setAllErrorCallbacks();
        virtual bool handleFileStreamErrorEvent();
        virtual bool handleApplicationErrorEvent();
        virtual bool handleNetworkErrorEvent();
        virtual bool handleWindowErrorEvent();
        virtual bool handleAllErrorEvents();
        virtual bool getOpenGLErrors();

        //Scenes
        void pushScene(sms::Scene* scene);

        //callbacks
        virtual bool onWindowFrameBufferSizeEvent(es::WindowFramebufferSizeEvent& e);
        virtual bool onWindowCloseEvent(es::WindowCloseEvent& e);
        virtual bool onWindowDropEvent(es::WindowDropEvent& e);
        virtual bool onWindowHasFocusEvent(es::WindowHasFocusEvent& e);
        virtual bool onWindowIconifyEvent(es::WindowIconifyEvent& e);
        virtual bool onWindowLostFocusEvent(es::WindowLostFocusEvent& e);
        virtual bool onWindowPositionEvent(es::WindowPositionEvent& e);
        virtual bool onWindowRefreshEvent(es::WindowRefreshEvent& e);
        virtual bool onWindowRestoredEvent(es::WindowRestoredEvent& e);
        virtual bool onWindowSizeEvent(es::WindowSizeEvent& e);
        virtual bool onApplicationErrorEvent(es::ApplicationErrorEvent& e);
        virtual bool onWindowErrorEvent(es::WindowErrorEvent& e);
        virtual bool onFiosErrorEvent(es::FileStreamErrorEvent& e);
        virtual bool onNetworkErrorEvent(es::NetworkErrorEvent& e);

        virtual void onEvent(es::Event& e) = 0;
        virtual void initApplication() = 0;
        virtual void applicationFunction() = 0;
    };
}
