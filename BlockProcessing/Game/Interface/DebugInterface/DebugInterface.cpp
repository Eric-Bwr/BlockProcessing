#include "DebugInterface.h"
#include "Game/Player/Player.h"
#include <sstream>
#include <iomanip>

static UIText* textLeft;
static UIText* textRight;
static std::ostringstream string;
static std::string gl;

//PARSE W H
void DebugInterface::init() {
    gl = (char*)glGetString(GL_VERSION);
    gl += "\n";
    gl += (char*)glGetString(GL_RENDERER);
    gl += "\nGLSL: ";
    gl += (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    textLeft = new UIText(string.str().data(), font, 30, 0, 0, 2000, 500, UITextMode::LEFT);
    textRight = new UIText(string.str().data(), font, 30, 0, 0, 2000, 500, UITextMode::RIGHT);
}

void DebugInterface::update() {
    string.str("");
    string << std::fixed << std::setprecision(2) << "X: " << Player::getX() << " Y: " << Player::getY() << " Z: " << Player::getZ() << "\n";
    string << "ChunkX: " << Player::chunkX << " ChunkY: " << Player::chunkY << " ChunkZ: " << Player::chunkZ << "\n";
    string << "OctreeX: " << getOctreeFromChunk(Player::chunkX) << " OctreeY: " << getOctreeFromChunk(Player::chunkY) << " OctreeZ: " << getOctreeFromChunk(Player::chunkZ) << "\n";
    string << gl << "\n";
    text->setText(string.str().data());
}

void DebugInterface::display(bool display) {
    if(display)
        UI.add(text, 2);
    else
        UI.remove(text);
}
