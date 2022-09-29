#include "DebugInterface.h"
#include "BlockProcessing/Game/Features/Entities/Player/Player.h"
#include "UI/UIComponents/Text/Structure/FontType.h"

void DebugInterface::init() {
    fps = 0;
    textLeft = new UIText("", font, 30, 0, 0, width, height, UITextMode::LEFT);
    textRight = new UIText(gl.data(), font, 30, 0, 0, width, height, UITextMode::RIGHT);
    backgroundsLeft.resize(5);
    backgroundsRight.resize(3);
    for (int i = 0; i < backgroundsLeft.size(); i++) {
        backgroundsLeft[i] = new UIImage(0, i * 30 - 1, 0, 0);
        backgroundsLeft[i]->color.setHex(0x000000, 0.5f);
    }
    for (int i = 0; i < backgroundsRight.size(); i++) {
        backgroundsRight[i] = new UIImage(0, i * 30 - 1, 0, 0);
        backgroundsRight[i]->color.setHex(0x000000, 0.5f);
    }
    std::string versionString = (std::string)(char*) glGetString(GL_VERSION) + "\n";
    float versionStringWidth = textRight->fontType->getTextWidth(versionString.data());
    backgroundsRight[0]->setPosition(width - versionStringWidth - 2, backgroundsRight[0]->getY());
    backgroundsRight[0]->setSize(versionStringWidth + 2, 30);
    stringRight += versionString;
    std::string rendererString = (std::string)(char *) glGetString(GL_RENDERER) + "\n";
    float rendererStringWidth = textRight->fontType->getTextWidth(rendererString.data());
    backgroundsRight[1]->setPosition(width - rendererStringWidth - 2, backgroundsRight[1]->getY());
    backgroundsRight[1]->setSize(rendererStringWidth + 2, 30);
    stringRight += rendererString;
    std::string glslString = "GLSL: " + (std::string)(char *) glGetString(GL_SHADING_LANGUAGE_VERSION);
    float glslStringWidth = textRight->fontType->getTextWidth(glslString.data());
    backgroundsRight[2]->setPosition(width - glslStringWidth - 2, backgroundsRight[2]->getY());
    backgroundsRight[2]->setSize(glslStringWidth + 2, 30);
    stringRight += glslString;
    textRight->setText(stringRight.data());
}

template<typename T>
std::string getStringPrecision(T value, int precision = 0) {
    auto string = std::to_string(value);
    return string.substr(0, string.find('.') + precision);
}

void DebugInterface::update(Player *player) {
    stringLeft.clear();
    stringLeft += "FPS: " + getStringPrecision(fps) + "\n";
    backgroundsLeft[0]->setSize(textLeft->fontType->getTextWidth(stringLeft.data()), 30);
    std::string posString = "X: " + getStringPrecision(player->getX(), 3) + " Y: " + getStringPrecision(player->getY(), 3) + " Z: " + getStringPrecision(player->getZ(), 3) + "\n";
    backgroundsLeft[1]->setSize(textLeft->fontType->getTextWidth(posString.data()), 30);
    stringLeft += posString;
    std::string posBlockString = "BlockX: " + getStringPrecision(player->blockX) + " BlockY: " + getStringPrecision(player->blockY) + " BlockZ: " + getStringPrecision(player->blockZ) + "\n";
    backgroundsLeft[2]->setSize(textLeft->fontType->getTextWidth(posBlockString.data()), 30);
    stringLeft += posBlockString;
    std::string posChunkString = "ChunkX: " + getStringPrecision(player->chunkX) + " ChunkY: " + getStringPrecision(player->chunkY) + " ChunkZ: " + getStringPrecision(player->chunkZ) + "\n";
    backgroundsLeft[3]->setSize(textLeft->fontType->getTextWidth(posChunkString.data()), 30);
    stringLeft += posChunkString;
    std::string posOctreeString = "OctreeX: " + getStringPrecision(player->octreeX / CHUNK_SIZE) + " OctreeY: " + getStringPrecision(player->octreeY / CHUNK_SIZE) + " OctreeZ: " + getStringPrecision(player->octreeZ / CHUNK_SIZE) + "\n";
    backgroundsLeft[4]->setSize(textLeft->fontType->getTextWidth(posOctreeString.data()), 30);
    stringLeft += posOctreeString;
    textLeft->setText(stringLeft.data());
}

void DebugInterface::setFPS(float inFps) {
    fps = inFps;
}

void DebugInterface::load() {
    for(auto background : backgroundsLeft)
        UI->add(background, 1);
    for(auto background : backgroundsRight)
        UI->add(background, 1);
    UI->add(textLeft, 2);
    UI->add(textRight, 2);
}

void DebugInterface::unload() {
    UI->remove(textLeft);
    UI->remove(textRight);
    for(auto background : backgroundsLeft)
        UI->remove(background);
    for(auto background : backgroundsRight)
        UI->remove(background);
}

DebugInterface::~DebugInterface() {
    UI->remove(textLeft);
    UI->remove(textRight);
    for(auto background : backgroundsLeft) {
        UI->remove(background);
        delete background;
    }
    for(auto background : backgroundsRight) {
        UI->remove(background);
        delete background;
    }
    delete textLeft;
    delete textRight;
}

void DebugInterface::onResize(int width, int height, float factorX, float factorY) {
    for(auto background : backgroundsRight)
        background->setPosition(width - background->width, background->positionY);
    textRight->setPosition(width - textRight->width, textRight->positionY);
}
