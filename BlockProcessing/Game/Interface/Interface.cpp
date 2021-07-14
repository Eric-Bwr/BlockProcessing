#include "Interface.h"

UIManager Interface::UI;
Font* Interface::font;

void Interface::init(int width, int height) {
    UI.init(width, height, true);
    font = new Font("../Assets/Fonts/Minecraft.otf");
}

void Interface::render() {
    UI.render();
}