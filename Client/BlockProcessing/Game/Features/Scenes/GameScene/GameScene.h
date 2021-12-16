#pragma once

#include "BlockProcessing/Game/Engine/Scene/Scene.h"
#include "BlockProcessing/Game/Features/Entities/Player/Player.h"
#include "BlockProcessing/Game/Engine/Command/CommandManager.h"
#include "BlockProcessing/Game/Features/Interfaces/GameInterfaces/DebugInterface/DebugInterface.h"
#include "BlockProcessing/Game/Features/Interfaces/GameInterfaces/ChatInterface/ChatInterface.h"
#include "BlockProcessing/Game/Features/Interfaces/GameInterfaces/CrosshairInterface/CrosshairInterface.h"
#include "BlockProcessing/Game/Features/Interfaces/GameInterfaces/GameMenuInterface/GameMenuInterface.h"
#include "BlockProcessing/Game/Features/Interfaces/GameInterfaces/OptionsMenuInterface/OptionsMenuInterface.h"
#include "BlockProcessing/Game/Features/Terrain/Debug/ChunkBorder/ChunkBorderVisualizer.h"
#include "BlockProcessing/Game/Features/Terrain/Debug/LinePoint/LinePointVisualizer.h"
#include "BlockProcessing/Game/Features/Terrain/Debug/Octree/OctreeVisualizer.h"
#include "BlockProcessing/Game/Features/Terrain/TerrainManager.h"
#include "BlockProcessing/Game/Features/Commands/CommandTP/CommandTP.h"
#include "BlockProcessing/Game/Features/Commands/CommandSpeed/CommandSpeed.h"
#include "BlockProcessing/Game/Features/Commands/CommandHelp/CommandHelp.h"

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

    TerrainManager terrainManager;

    OptionsMenuInterface optionsMenuInterface;

    GLFWwindow* window;
    Mat4d view;
    Mat4f viewf;
    Mat4d projectionView;
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