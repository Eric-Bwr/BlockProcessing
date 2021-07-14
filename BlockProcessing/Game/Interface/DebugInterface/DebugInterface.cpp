#include "DebugInterface.h"
#include <sstream>
#include <iomanip>

static UIText* text;
static std::ostringstream string;
static float x = 0, y = 0, z = 0;

void DebugInterface::init() {
    text = new UIText(string.str().data(), font, 40, 0, 0, 1000, 500, UITextMode::LEFT);
}

void DebugInterface::setXYZ(float xIn, float yIn, float zIn) {
    x = xIn;
    y = yIn;
    z = zIn;
}

void DebugInterface::update() {
    string.str("");
    string << std::fixed << std::setprecision(2) << "X: " << x << " Y: " << y << " Z: " << z;
    text->setText(string.str().data());
}

void DebugInterface::display(bool display) {
    if(display)
        UI.add(text, 2);
    else
        UI.remove(text);
}
