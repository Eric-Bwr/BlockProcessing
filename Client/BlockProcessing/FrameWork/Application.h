#pragma once

#include <csignal>
#include <ctime>
#include "BlockProcessing/FrameWork/Window/Window.h"
#include "BlockProcessing/Game/Terrain/TerrainManager.h"
#include "BlockProcessing/Game/Player/Player.h"
#include "BlockProcessing/Game/Debug/ChunkBorder/ChunkBorderVisualizer.h"
#include "BlockProcessing/Game/Debug/Octree/OctreeVisualizer.h"
#include "BlockProcessing/Game/Debug/LinePoint/LinePointVisualizer.h"
#include "BlockProcessing/Game/Player/PlayerBlockOutline.h"

#include "BlockProcessing/Game/BlockProcessing.h"

class Application {
public:
    void preInit();
    void init();
    void run();
    void end();

    Window window;
    BlockProcessing* blockProcessing;
    int width, height;
    int frames = 0;
    double lastTime = 0, currentTime = 0;
    double frameDeltaTime = 0;

    void onKey(int key, int scancode, int action, int mods);
    void onChar(unsigned int key);
    void onMousePosition(double x, double y);
    void onMouseButton(int button, int action, int mods);
    void onMouseScroll(double x, double y);
private:
    void initCallbacks();
};

extern Application application;