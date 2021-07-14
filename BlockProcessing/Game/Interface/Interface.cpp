#include "Interface.h"

UIManager Interface::UI;
Font* Interface::font;

void Interface::init(int width, int height, bool scaleOnResize) {
    UI.init(width, height, scaleOnResize);
    font = new Font("../Assets/Fonts/Minecraft.otf");
}

void Interface::render() {
    UI.render();
}