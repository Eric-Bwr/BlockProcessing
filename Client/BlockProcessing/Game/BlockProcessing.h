#pragma once

#include "BlockProcessing/FrameWork/Window/Window.h"

#include "BlockProcessing/Game/Command/CommandManager.h"
#include "BlockProcessing/Game/Command/Commands/CommandTP/CommandTP.h"
#include "BlockProcessing/Game/Debug/ChunkBorder/ChunkBorderVisualizer.h"
#include "BlockProcessing/Game/Debug/LinePoint/LinePointVisualizer.h"
#include "BlockProcessing/Game/Debug/Octree/OctreeVisualizer.h"
#include "BlockProcessing/Game/Interface/InterfaceManager.h"
#include "BlockProcessing/Game/Interface/Interfaces/DebugInterface/DebugInterface.h"
#include "BlockProcessing/Game/Interface/Interfaces/ChatInterface/ChatInterface.h"
#include "BlockProcessing/Game/Interface/Interfaces/CrosshairInterface/CrosshairInterface.h"
#include "BlockProcessing/Game/Player/Player.h"
#include "BlockProcessing/Game/PostProcess/PostProcessManager.h"
#include "BlockProcessing/Game/Terrain/Block/BlockManager.h"
#include "BlockProcessing/Game/Terrain/Chunk/ChunkManager.h"
#include "BlockProcessing/Game/Terrain/Cube/CubeManager.h"
#include "BlockProcessing/Game/Terrain/World/WorldManager.h"
#include "BlockProcessing/Game/Terrain/TerrainManager.h"
#include "BlockProcessing/Game/Network/NetworkManager.h"

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

    InterfaceManager interfaceManager;
    CommandManager commandManager;
    DebugInterface debugInterface;
    ChatInterface chatInterface;
    CrosshairInterface crosshairInterface;

    Player player;

    ChunkBorderVisualizer chunkBorderVisualizer;
    LinePointVisualizer linePointVisualizer;
    OctreeVisualizer octreeVisualizer;

    NetworkManager networkManager;

    void onKey(int key, int scancode, int action, int mods);
    void onChar(unsigned int key);
    void onMousePosition(double x, double y);
    void onMouseButton(int button, int action, int mods);
    void onMouseScroll(double x, double y);
};