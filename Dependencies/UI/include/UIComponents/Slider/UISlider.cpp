#include "UISlider.h"

UISlider::UISlider(float width, float height)
        : UISlider(0, 0, width, height, 0, 0, 10) {}

UISlider::UISlider(float positionX, float positionY, float width, float height)
        : UISlider(positionX, positionY, width, height, 0, 0, 10) {}

UISlider::UISlider(float positionX, float positionY, float width, float height, float value, float min, float max) {
    type = UIComponentType::UISLIDER;
    this->value = value;
    this->min = min;
    this->max = max;
    this->positionX = positionX;
    this->positionY = positionY;
    this->width = width;
    this->height = height;
    this->bgColor.standard = COLOR_BLUE;
    this->bgColor.hover = COLOR_BLUE.darker();
    this->bgColor.pressed = COLOR_BLUE.darker().darker();
    this->slideColor.standard = COLOR_GREEN;
    this->slideColor.hover = COLOR_GREEN.darker();
    this->slideColor.pressed = COLOR_GREEN.darker().darker();
    this->dragColor.standard = COLOR_RED;
    this->dragColor.hover = COLOR_RED.darker();
    this->dragColor.pressed = COLOR_RED.darker().darker();
    this->renderWidth = 0;
    update();
    for (int i = 0; i < 18; i++)
        sizes[i] = 0;
    for (int i = 0; i < 3; i++)
        modes[i] = 0;
    bgMesh.load(positionX, positionY, width, height, 0);
    slideMesh.load(positionX, positionY, renderX, height, 0);
    dragMesh.load(0, 0, 0, 0, 0);
    this->texture = nullptr;
}

void UISlider::setTexture(Texture *texture) {
    this->texture = texture;
    for(int& mode : modes) {
        if (texture == nullptr) {
            if (mode == 3)
                mode = 1;
            else if (mode == 2)
                mode = 0;
        } else {
            if (mode == 0)
                mode = 2;
            else if (mode == 1)
                mode = 3;
        }
    }
}

void UISlider::setBackgroundColor(const UIColor& standardColor, const UIColor& hoverColor, const UIColor& pressedColor) {
    this->bgColor.standard = standardColor;
    this->bgColor.hover = hoverColor;
    this->bgColor.pressed = pressedColor;
    if (modes[0] == 0)
        modes[0] = 1;
    else if (modes[0] == 2)
        modes[0] = 3;
    bgMesh.loadPosition(positionX, positionY, width, height, modes[0]);
}

void UISlider::setBackgroundCoords(float buttonX, float buttonY, float buttonWidth, float buttonHeight, float hoverX, float hoverY, float hoverWidth, float hoverHeight, float pressedX, float pressedY, float pressedWidth, float pressedHeight) {
    if (modes[0] == 0)
        modes[0] = 2;
    else if (modes[0] == 1)
        modes[0] = 3;
    bgMesh.load(positionX, positionY, width, height, modes[0], texture->getWidth(), texture->getHeight(), buttonX, buttonY, buttonWidth, buttonHeight, hoverX, hoverY, hoverWidth, hoverHeight, pressedX, pressedY, pressedWidth, pressedHeight);
}

void UISlider::setSlideColor(const UIColor& standardColor, const UIColor& hoverColor, const UIColor& pressedColor) {
    this->slideColor.standard = standardColor;
    this->slideColor.hover = hoverColor;
    this->slideColor.pressed = pressedColor;
    if (modes[1] == 0)
        modes[1] = 1;
    else if (modes[1] == 2)
        modes[1] = 3;
    updateSlideMesh();
}

void UISlider::setSlideCoords(float buttonX, float buttonY, float buttonWidth, float buttonHeight, float hoverX, float hoverY, float hoverWidth, float hoverHeight, float pressedX, float pressedY, float pressedWidth, float pressedHeight) {
    sizes[6] = buttonX;
    sizes[7] = buttonY;
    sizes[8] = buttonWidth;
    sizes[9] = buttonHeight;
    sizes[10] = hoverX;
    sizes[11] = hoverY;
    sizes[12] = hoverWidth;
    sizes[13] = hoverHeight;
    sizes[14] = pressedX;
    sizes[15] = pressedY;
    sizes[16] = pressedWidth;
    sizes[17] = pressedHeight;
    if (modes[1] == 0)
        modes[1] = 2;
    else if (modes[1] == 1)
        modes[1] = 3;
    update();
    updateSlideMesh();
}

void UISlider::setDragColor(const UIColor& standardColor, const UIColor& hoverColor, const UIColor& pressedColor) {
    this->dragColor.standard = standardColor;
    this->dragColor.hover = hoverColor;
    this->dragColor.pressed = pressedColor;
    if (modes[2] == 0)
        modes[2] = 1;
    else if (modes[2] == 2)
        modes[2] = 3;
    updateDragMesh();
}

void UISlider::setDragCoords(float dragWidth, float dragHeight, float buttonX, float buttonY, float buttonWidth, float buttonHeight, float hoverX, float hoverY, float hoverWidth, float hoverHeight, float pressedX, float pressedY, float pressedWidth, float pressedHeight) {
    sizes[0] = dragWidth;
    sizes[1] = dragHeight;
    if (modes[2] == 0)
        modes[2] = 2;
    else if (modes[2] == 1)
        modes[2] = 3;
    renderWidth = width - dragWidth;
    update();
    updateSlideMesh();
    dragMesh.load(positionX + renderX, positionY + height / 2 - sizes[1] / 2, sizes[0], sizes[1], positionX + renderX, positionY + height / 2 - sizes[1] / 2, sizes[0], sizes[1], positionX + renderX, positionY + height / 2 - sizes[1] / 2, sizes[0], sizes[1], modes[2], texture->getWidth(), texture->getHeight(), buttonX, buttonY, buttonWidth, buttonHeight, hoverX, hoverY, hoverWidth, hoverHeight, pressedX, pressedY, pressedWidth, pressedHeight);
}

void UISlider::setPosition(float positionX, float positionY) {
    this->positionX = positionX;
    this->positionY = positionY;
    update();
    updateBackgroundMesh();
    updateSlideMesh();
    updateDragMesh();
}

void UISlider::setSize(float width, float height) {
    this->width = width;
    this->height = height;
    update();
    updateBackgroundMesh();
    updateSlideMesh();
    updateDragMesh();
}

void UISlider::setBounds(float positionX, float positionY, float width, float height) {
    this->positionX = positionX;
    this->positionY = positionY;
    sizes[0] *= (width / this->width);
    renderWidth = width - sizes[0];
    sizes[1] *= (height / this->height);
    this->width = width;
    this->height = height;
    update();
    updateBackgroundMesh();
    updateSlideMesh();
    updateDragMesh();
}

void UISlider::setRadii(float radii, bool upperLeft, bool lowerLeft, bool upperRight, bool lowerRight) {
    dragMesh.setRadii(radii, upperLeft, lowerLeft, upperRight, lowerRight);
    slideMesh.setRadii(radii, upperLeft, lowerLeft, false, false);
    bgMesh.setRadii(radii, upperLeft, lowerLeft, upperRight, lowerRight);
}

void UISlider::mousePositionInput(double x, double y) {
    hovered = COMPONENT_HOVERED(x, y);
    if (dragging)
        drag(x - positionX - sizes[0] / 2);
}

void UISlider::mouseButtonInput(int action) {
    bool previous = hovered;
    if (action == INPUT_PRESSED) {
        if (hovered)
            dragging = true;
    } else if (action == INPUT_RELEASED)
        dragging = false;
    if (previous && !hovered || !previous && hovered)
        if (callback != nullptr)
            (*callback)(dragging, hovered, value);
}

void UISlider::drag(float rx) {
    value = getInc(min + (max - min) * (rx / renderWidth));
    if (value > max)
        value = max;
    else if (value < min)
        value = min;
    renderX = getInc(renderWidth * (value - min) / (max - min));
    updateSlideMesh();
    updateDragMesh();
    if (callback != nullptr)
            (*callback)(dragging, hovered, value);
}

float UISlider::getInc(float val) const {
    if (floating)
        return val;
    float one = 1.0f / increment;
    return std::round(val * one) / one;
}

void UISlider::update() {
    float norm = (value - min) / (max - min);
    renderX = renderWidth * norm;
}

void UISlider::updateBackgroundMesh() {
    bgMesh.loadPosition(positionX, positionY, width, height, modes[0]);
}

void UISlider::updateSlideMesh() {
    if (modes[1] == 2 || modes[1] == 3)
        slideMesh.load(positionX, positionY, renderX + sizes[0] / 2, height, modes[1], texture->getWidth(), texture->getHeight(), sizes[6], sizes[7], ((renderX + sizes[0] / 2) / width) * sizes[8], sizes[9], sizes[10], sizes[11], ((renderX + sizes[0] / 2) / width) * sizes[12], sizes[13], sizes[14], sizes[15], ((renderX + sizes[0] / 2) / width) * sizes[16], sizes[17]);
    else
        slideMesh.loadPosition(positionX, positionY, renderX, height, modes[1]);
}

void UISlider::updateDragMesh() {
    dragMesh.loadPosition(positionX + renderX, positionY + height / 2 - sizes[1] / 2, sizes[0], sizes[1], positionX + renderX, positionY + height / 2 - sizes[1] / 2, sizes[0], sizes[1], positionX + renderX, positionY + height / 2 - sizes[1] / 2, sizes[0], sizes[1], modes[2]);
}

void UISlider::setMin(float m) {
    min = m;
    update();
}

void UISlider::setMax(float m) {
    max = m;
    update();
}

void UISlider::setValue(float v) {
    value = v;
    update();
}