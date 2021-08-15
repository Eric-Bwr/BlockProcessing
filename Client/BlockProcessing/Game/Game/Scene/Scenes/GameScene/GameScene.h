#pragma once

#include "BlockProcessing/Game/Game/Scene/Scene.h"
#include "BlockProcessing/Game/Game/Entity/Entities/Player/Player.h"
#include "BlockProcessing/Game/Game/Command/CommandManager.h"
#include "BlockProcessing/Game/Game/Interface/Interfaces/GameInterfaces/DebugInterface/DebugInterface.h"
#include "BlockProcessing/Game/Game/Interface/Interfaces/GameInterfaces/ChatInterface/ChatInterface.h"
#include "BlockProcessing/Game/Game/Interface/Interfaces/GameInterfaces/CrosshairInterface/CrosshairInterface.h"
#include "BlockProcessing/Game/Game/Interface/Interfaces/GameInterfaces/GameMenuInterface/GameMenuInterface.h"
#include "BlockProcessing/Game/Game/Interface/Interfaces/GameInterfaces/OptionsMenuInterface/OptionsMenuInterface.h"
#include "BlockProcessing/Game/Game/Terrain/Debug/ChunkBorder/ChunkBorderVisualizer.h"
#include "BlockProcessing/Game/Game/Terrain/Debug/LinePoint/LinePointVisualizer.h"
#include "BlockProcessing/Game/Game/Terrain/Debug/Octree/OctreeVisualizer.h"
#include "BlockProcessing/Game/Game/Terrain/Block/BlockManager.h"
#include "BlockProcessing/Game/Game/Terrain/Chunk/ChunkManager.h"
#include "BlockProcessing/Game/Game/Terrain/Cube/CubeManager.h"
#include "BlockProcessing/Game/Game/Terrain/World/WorldManager.h"
#include "BlockProcessing/Game/Game/Terrain/TerrainManager.h"
#include "BlockProcessing/Game/Game/Command/Commands/CommandTP/CommandTP.h"

class OptionsMenuInterface;

struct GLFWwindow;

class GameScene : public Scene {
public:
    void init() override;
    void load() override;
    void unload() override;
    void update(double deltaFrameTime) override;
    void render(double deltaFrameTime) override;
    void updateProjection(float fov);
    void onKey(int key, int scancode, int action, int mods) override;
    void onMousePosition(double x, double y) override;
    void onMouseButton(int button, int action, int mods) override;
    void onResize(bool show, int width, int height) override;
    int getID() override;
    ~GameScene() override;

    Player player;
    ChatInterface chatInterface;
    CommandManager commandManager;
    DebugInterface debugInterface;
    CrosshairInterface crosshairInterface;
    GameMenuInterface gameMenuInterface;

    ChunkBorderVisualizer chunkBorderVisualizer;
    LinePointVisualizer linePointVisualizer;
    OctreeVisualizer octreeVisualizer;

    CubeManager cubeManager;
    BlockManager blockManager;
    ChunkManager chunkManager;
    WorldManager worldManager;
    TerrainManager terrainManager;

    OptionsMenuInterface optionsMenuInterface;

    GLFWwindow* window;
    Mat4 view;
    Mat4 projectionView;
    int width, height;
    bool mode = false;
    bool modeCursor = false;
    bool modeGui = false;
    bool leftControl = false;
    bool collision = false;
    bool debug = false;
    bool chat = false;
    bool gameMenu = false;
    bool optionsMenu = false;
    bool crosshair = false;
    bool inInterface = false;
    bool zoom = true;
    bool wireFrame = false;
};