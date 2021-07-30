#pragma once

#include <csignal>
#include <ctime>
#include "Window/Window.h"
#include "Game/Terrain/TerrainManager.h"
#include "Game/Player/Player.h"
#include "Game/Debug/ChunkBorder/ChunkBorderVisualizer.h"
#include "Game/Debug/Octree/OctreeVisualizer.h"
#include "Game/Debug/LinePoint/LinePoint.h"
#include "Game/Interface/DebugInterface/DebugInterface.h"
#include "Game/Interface/ChatInterface/ChatInterface.h"
#include "Game/Interface/CrosshairInterface/CrosshairInterface.h"
#include "Game/Command/CommandManager.h"
#include "Game/Player/PlayerBlockOutline.h"

class Application {
public:
    void preInit();
    void init();
    void run();
    void update();
    void render();
    void end();
public:
    void initCallbacks();
    void onKey(int key, int scancode, int action, int mods);
    void onChar(unsigned int key);
    void onMousePosition(double x, double y) const;
    void onMouseButton(int button, int action, int mods);
    void onMouseScroll(double x, double y);
private:
    Window window;
    int frames = 0;
    double lastTime = 0, currentTime = 0;
    double frameDeltaTime = 0;
    bool mode = false;
    bool modeGui = false;
    bool modeCursor = false;
    bool leftControl = false;
    bool alt = false;
    bool collision = false;
    bool debug = false;
    bool chat = false;
    bool crosshair = true;
    bool allowCommand = true;
    float width, height;
    float cameraSpeed = 7.0f;
    bool zoom = true;
    bool wireFrame = false;
    float span = 0.0f;
    float offset = 4.5;
    float zoomLevel = 45.0f, prevZoomLevel = 45.0f, zoomSpeed = 1.0f;
    float maxZoom = 45.0f, minZoom = 1.0f;

    float fov = 45.0f;
    Mat4 projection;
    Mat4 view;
    Mat4 projectionView;
};

extern Application application;