#pragma once

#include "Window/Window.h"
#include "Game/Terrain/TerrainManager.h"
#include "Game/Player/Player.h"
#include "Game/Debug/ChunkBorder/ChunkBorderManager.h"
#include "Game/Debug/Octree/OctreeVisualizer.h"
#include "Game/Debug/LinePoint/LinePoint.h"
#include "Game/Interface/DebugInterface/DebugInterface.h"
#include "Game/Interface/ChatInterface/ChatInterface.h"
#include "Game/Command/CommandManager.h"

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
    void onFrameBufferSize(int width, int height);
    void onKey(int key, int scancode, int action, int mods);
    void onChar(unsigned int key);
    void onMousePosition(double x, double y) const;
    void onMouseButton(int button, int action, int mods);
    void onMouseScroll(double x, double y);
private:
    Window window;
    ChatInterface chatInterface;
    DebugInterface debugInterface;
    bool mode = false;
    bool modeGui = false;
    bool modeCursor = false;
    bool leftControl = false;
    bool alt = false;
    bool collision = false;
    bool debug = false;
    bool chat = false;
    bool allowCommand = true;
    float width, height;
    float cameraSpeed = 7.0f;
    bool zoom = true;
    bool wireFrame = false;
    float span = 0.0f;
    float offset = 4.5 * TERRAIN_SIZE;
    float zoomLevel = 45.0f, prevZoomLevel = 45.0f, zoomSpeed = 1.0f;
    float maxZoom = 45.0f, minZoom = 1.0f;

    float fov = 45.0f;
    Mat4f projection;
    Mat4f view;
    Mat4f projectionView;
};

extern Application application;