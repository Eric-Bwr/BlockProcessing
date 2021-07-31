#pragma once

#include "FrameWork/Window/Window.h"

#include "Command/CommandManager.h"
#include "Game/Command/Commands/CommandTP/CommandTP.h"
#include "Debug/ChunkBorder/ChunkBorderVisualizer.h"
#include "Game/Debug/LinePoint/LinePointVisualizer.h"
#include "Debug/Octree/OctreeVisualizer.h"
#include "Interface/InterfaceManager.h"
#include "Interface/Interfaces/DebugInterface/DebugInterface.h"
#include "Interface/Interfaces/ChatInterface/ChatInterface.h"
#include "Interface/Interfaces/CrosshairInterface/CrosshairInterface.h"
#include "Player/Player.h"
#include "PostProcess/PostProcessManager.h"
#include "Terrain/Block/BlockManager.h"
#include "Terrain/Chunk/ChunkManager.h"
#include "Terrain/Cube/CubeManager.h"
#include "Terrain/World/WorldManager.h"
#include "Terrain/TerrainManager.h"

class BlockProcessing {
public:
    void init(GLFWwindow* window, int width, int height);
    void update(double frameDeltaTime);
    void render(double frameDeltaTime);
    void end();
private:
    GLFWwindow* window;
    int width, height;
    bool wireFrame = false;
    bool alt = false;
    bool collision = false;
    bool debug = false;
    bool chat = false;
    bool zoom = true;
    bool crosshair = true;
    bool allowCommand = true;
    bool mode = false;
    bool modeGui = false;
    bool modeCursor = false;
    bool leftControl = false;
    float zoomLevel = 45.0f, prevZoomLevel = 45.0f;
    float fov = 45.0f;
    Mat4 projection;
    Mat4 view;
    Mat4 projectionView;
public:
    CubeManager cubeManager;
    BlockManager blockManager;
    ChunkManager chunkManager;
    WorldManager worldManager;
    TerrainManager terrainManager;

    InterfaceManager interface;
    CommandManager commandManager;
    DebugInterface debugInterface;
    ChatInterface chatInterface;
    CrosshairInterface crosshairInterface;

    Player player;

    ChunkBorderVisualizer chunkBorderVisualizer;
    LinePointVisualizer linePointVisualizer;
    OctreeVisualizer octreeVisualizer;

    void onKey(int key, int scancode, int action, int mods);
    void onChar(unsigned int key);
    void onMousePosition(double x, double y);
    void onMouseButton(int button, int action, int mods);
    void onMouseScroll(double x, double y);
};