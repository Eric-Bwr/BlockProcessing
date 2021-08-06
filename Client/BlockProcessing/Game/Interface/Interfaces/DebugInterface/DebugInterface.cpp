#include "DebugInterface.h"
#include "BlockProcessing/Game/Player/Player.h"
#include <iomanip>

void DebugInterface::init() {
    fps = 0;
    gl = (char*)glGetString(GL_VERSION);
    gl += "\n";
    gl += (char*)glGetString(GL_RENDERER);
    gl += "\nGLSL: ";
    gl += (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    textLeft = new UIText("", font, 30, 0, 0, 2000, 500, UITextMode::LEFT);
    textRight = new UIText(gl.data(), font, 30, 0, 0, width, 500, UITextMode::RIGHT);
}

void DebugInterface::update(Player* player) {
    stringLeft.str("");
    stringLeft << "FPS: " << fps << "\n";
    stringLeft << std::fixed << std::setprecision(2) << "X: " << player->getX() << " Y: " << player->getY() << " Z: " << player->getZ() << "\n";
    stringLeft << "BlockX: " << player->blockX << " BlockY: " << player->blockY << " BlockZ: " << player->blockZ << "\n";
    stringLeft << "ChunkX: " << player->chunkX << " ChunkY: " << player->chunkY << " ChunkZ: " << player->chunkZ << "\n";
    stringLeft << "OctreeX: " << player->octreeX << " OctreeY: " << player->octreeY << " OctreeZ: " << player->octreeZ << "\n";
    textLeft->setText(stringLeft.str().data());
}

void DebugInterface::setFPS(float inFps) {
    fps = inFps;
}

void DebugInterface::display(bool display) {
    if(display) {
        UI->add(textLeft, 2);
        UI->add(textRight, 2);
    } else {
        UI->remove(textLeft);
        UI->remove(textRight);
    }
}

DebugInterface::~DebugInterface() {
    UI->remove(textLeft);
    UI->remove(textRight);
    delete textLeft;
    delete textRight;
}