#pragma once

#include <csignal>
#include <ctime>
#include "Window/Window.h"
#include "Game/Terrain/TerrainManager.h"
#include "Game/Player/Player.h"
#include "Game/Debug/ChunkBorder/ChunkBorderVisualizer.h"
#include "Game/Debug/Octree/OctreeVisualizer.h"
#include "Game/Debug/LinePoint/LinePointVisualizer.h"
#include "Game/Player/PlayerBlockOutline.h"

#include "Game/BlockProcessing.h"

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