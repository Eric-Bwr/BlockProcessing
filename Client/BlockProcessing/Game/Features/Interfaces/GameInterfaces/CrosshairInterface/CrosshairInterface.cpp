#include "CrosshairInterface.h"

void CrosshairInterface::init() {
    crosshairTexture = new UITexture(TEXTURE_GUI);
    crosshair = new UIImage(width / 2 - size / 2, height / 2 - size / 2, size, size);
    crosshair->setTexture(crosshairTexture, 201, 0, 15, 15);
    crosshair->setColor({1, 1, 1, 0.5});
}

void CrosshairInterface::load() {
    UI->add(crosshair, 2);
}

void CrosshairInterface::unload() {
    UI->remove(crosshair);
}

void CrosshairInterface::onResize(int width, int height, float factorX, float factorY) {
    crosshair->setPosition(crosshair->getX() * factorX, crosshair->getY() * factorY);
}

CrosshairInterface::~CrosshairInterface() {
    UI->remove(crosshair);
    delete crosshairTexture;
    delete crosshair;
}