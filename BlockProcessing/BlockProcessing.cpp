#include "Game/Terrain/Cube/CubeManager.h"
#include <FrameBuffer.h>
#include <Game/Terrain/TerrainManager.h>
#include <Game/Terrain/Chunk/ChunkManager.h>
#include <Game/Player/Player.h>
#include <Game/Event/Events/amples.h>
#include <Game/Debug/ChunkBorder/ChunkBorderManager.h>
#include "Engine/PostProcess/PostProcessManager.h"
#include "Engine/3D/Model3D/Model3D.h"
#include "Game/Debug/Octree/OctreeVisualizer.h"
#include "Game/Terrain/Octree/OctreeNode.h"
#include "Game/Debug/LinePoint/LinePoint.h"

#include "FrameWork/WindowHandler.h"

int main() {
    WindowHandler windowHandler;
    while (windowHandler.window.windowIsAlive()){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        windowHandler.window.updateWindow();
    }
    windowHandler.window.destroyWindow();
}