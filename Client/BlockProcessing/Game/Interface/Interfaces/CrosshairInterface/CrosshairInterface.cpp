#include "CrosshairInterface.h"
#include "BlockProcessing/Paths.h"

void CrosshairInterface::init() {
    crosshairTexture = new UITexture(TEXTURE_ICONS);
    crosshair = new UIImage(width / 2 - size / 2, height / 2 - size / 2, size, size);
    crosshair->setTexture(crosshairTexture, 0, 0, 15, 15);
    crosshair->setColor({1, 1, 1, 0.5});
}

void CrosshairInterface::display(bool display) {
    if(display)
        UI->add(crosshair, 2);
    else
        UI->remove(crosshair);
}

CrosshairInterface::~CrosshairInterface() {
    delete crosshairTexture;
    delete crosshair;
}