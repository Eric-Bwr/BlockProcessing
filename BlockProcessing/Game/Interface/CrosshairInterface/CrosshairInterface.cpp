#include "CrosshairInterface.h"
#include "../../../Paths.h"

static UITexture* crosshairTexture;
static UIImage* crosshair;

void CrosshairInterface::init() {
    crosshairTexture = new UITexture(TEXTURE_ICONS);
    crosshair = new UIImage(Interface::UI.getWidth() / 2 - size / 2, Interface::UI.getHeight() / 2 - size / 2, size, size);
    crosshair->setTexture(crosshairTexture, 0, 0, 15, 15);
    crosshair->setColor({1, 1, 1, 0.5});
}

void CrosshairInterface::display(bool display) {
    if(display)
        UI.add(crosshair, 2);
    else
        UI.remove(crosshair);
}

CrosshairInterface::~CrosshairInterface() {
    delete crosshairTexture;
    delete crosshair;
}