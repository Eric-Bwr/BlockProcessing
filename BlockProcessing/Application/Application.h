#pragma once

#include <iostream>
#include "Engine/Window/Window.h"
#include "Game/Terrain/Cube/CubeManager.h"
#include <FrameBuffer.h>
#include <TimeRepresentationSystem/Clock.h>
#include <Game/Terrain/TerrainManager.h>
#include <Game/Terrain/Chunk/ChunkManager.h>
#include <Game/Player/Player.h>
#include <Game/Event/Events/amples.h>
#include <Game/Debug/ChunkBorder/ChunkBorderManager.h>
#include "Engine/3D/Camera/CameraFirstPerson.h"
#include "Engine/PostProcess/PostProcessManager.h"
#include "Engine/3D/Model3D/Model3D.h"
#include "Game/Debug/Octree/OctreeVisualizer.h"
#include "Game/Terrain/Octree/OctreeNode.h"
#include "Game/Debug/LinePoint/LinePoint.h"

class App {
private:
    int argc = 0;
    char** argv;
private:
    trs::clock::Clock updateClock;
    trs::clock::Clock renderClock;

    std::shared_ptr<cs::BasicCallback<void, es::Event&>> eventCallback;


    trs::clock::Clock timer;
    //------------------------------------------------------------------------------------------------------------------------------------------

    bool mode = false;
    bool modeGui = false;
    bool modeCursor = false;
    bool leftControl = false;
    bool alt = false;
    bool collision = false;
    bool showCoords = false;
    float width, height;
    float cameraSpeed = 7.0f;
    bool zoom = true;
    bool wireFrame = false;
    float span = 0.0f;
    float incr = 1.0f;
    float offset = 4.5 * TERRAIN_SIZE;
    Player* player;

    amples* d;
    OctreeNode* testNode;

    float fov = 45.0f;
    Mat4f projection;
    Mat4f view;
    Mat4f projectionView;
private:
    virtual void preInit() = 0;
    void _preInit();

    virtual void init() = 0;
    void _init();

    virtual void onEvent(es::Event& e) = 0;
    void _onEvent(es::Event& e);

    bool onWindowClose(es::WindowCloseEvent& e);
    bool onWindowResize(es::WindowSizeEvent& e);
    bool onKeyPressed(es::KeyPressedEvent& e);
    bool onKeyReleased(es::KeyReleasedEvent& e);
    bool onMouseButtonPressed(es::MouseButtonPressedEvent& e);
    bool onMouseButtonReleased(es::MouseButtonReleasedEvent& e);
    bool onMouseMove(es::CursorPositionEvent& e);
    bool onMouseUpWheel(es::MouseUpScrollEvent& e);
    bool onMouseDownWheel(es::MouseDownScrollEvent& e);
    bool onChar(es::CharEvent& e);

    virtual void update(double& gameTime) = 0;
    void _update(double& gameTime);

    virtual void render(double& gameTime) = 0;
    void _render(double& gameTime);

    virtual void save() = 0;
    void _save();

    virtual void load() = 0;
    void _load();
private:
    bool showDebugInfoInConsole = false;
    bool debugMode = false;
    std::chrono::duration<double> elapsed;
    double gameTime;
    static double timeScale;
    double deltaU = 0, deltaF = 0;
    uint64_t targetTicks = 0, actualTicks = 0;
    uint64_t targetFrames = 0, actualFrames = 0;
    uint64_t ticks = 0, frames = 0, totalTicks = 0;

    bool running = false;
    bool windowMinimized = false;

    void gameLoop();
protected:
    Window* appWindow;
public:
    App();
    App(int argc, char** argv);
    ~App();

    void createWindow(const char* iconPath, const char* title, int width, int height);
    void setErrorCallbacks();

    void run();
private:
    void setGLFWCallacks();
};

App* CreateApplication();