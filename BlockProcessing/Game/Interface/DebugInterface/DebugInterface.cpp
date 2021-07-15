#include "DebugInterface.h"
#include "Game/Player/Player.h"
#include <sstream>
#include <iomanip>

static UIText* textLeft;
static UIText* textRight;
static std::ostringstream stringLeft;
static std::string gl;

void DebugInterface::init() {
    gl = (char*)glGetString(GL_VERSION);
    gl += "\n";
    gl += (char*)glGetString(GL_RENDERER);
    gl += "\nGLSL: ";
    gl += (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    textLeft = new UIText("", font, 30, 0, 0, 2000, 500, UITextMode::LEFT);
    textRight = new UIText(gl.data(), font, 30, 0, 0, UI.getWidth(), 500, UITextMode::RIGHT);
}

void DebugInterface::update() {
    stringLeft.str("");
    stringLeft << std::fixed << std::setprecision(2) << "X: " << Player::getX() << " Y: " << Player::getY() << " Z: " << Player::getZ() << "\n";
    stringLeft << "ChunkX: " << Player::chunkX << " ChunkY: " << Player::chunkY << " ChunkZ: " << Player::chunkZ << "\n";
    stringLeft << "OctreeX: " << getOctreeFromChunk(Player::chunkX) << " OctreeY: " << getOctreeFromChunk(Player::chunkY) << " OctreeZ: " << getOctreeFromChunk(Player::chunkZ) << "\n";
    textLeft->setText(stringLeft.str().data());
}

void DebugInterface::display(bool display) {
    if(display) {
        UI.add(textLeft, 2);
        UI.add(textRight, 2);
    } else {
        UI.remove(textLeft);
        UI.remove(textRight);
    }
}