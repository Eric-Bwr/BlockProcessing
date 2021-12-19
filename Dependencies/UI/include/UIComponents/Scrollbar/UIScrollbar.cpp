#include "../../UIColor.h"

#include "UIScrollbar.h"

#define MAX(a, b) (a > b ? a : b)

UIScrollbar::UIScrollbar(UIComponent *target, float width, float height, Orientation orientation)
		: UIScrollbar(target, 0, 0, width, height, orientation) {
}

UIScrollbar::UIScrollbar(UIComponent *target, float positionX, float positionY, float width, float height, Orientation orientation) {
	type = UIComponentType::UISCROLLBAR;
	this->target = target;
	this->positionX = positionX;
	this->positionY = positionY;
	this->width = width;
	this->height = height;
	this->barFgColor = COLOR_GREEN;
	this->barBgColor = COLOR_RED;
	this->orientation = orientation;
	barY = 0;
	barWidth = 10;
	if (orientation == Orientation::VERTICAL)
		barHeight = height;
	else
		barHeight = width;

	barBgMesh.load(positionX, positionY, width, height, 0);
	barFgMesh.load(positionX, positionY, width, height, 0);
	calc();
}

void UIScrollbar::calc() {
	if (orientation == Orientation::VERTICAL) {
		auto h = MAX(target->height, height);
		barHeight = height * height / h;

		if (barY <= 0)
			barY = 0;
		if (barY >= height - barHeight)
			barY = height - barHeight;

		target->setBounds(positionX, positionY - barY * (h / height), width - barWidth, h);

		barBgMesh.load(positionX + width - barWidth, positionY, barWidth, height, 0);
		barFgMesh.load(positionX + width - barWidth, positionY + barY, barWidth, barHeight, 0);
	} else {
		auto w = MAX(target->width, width);
		barHeight = width * width / w;

		if (barY <= 0)
			barY = 0;
		if (barY >= width - barHeight)
			barY = width - barHeight;

		target->setBounds(positionX - barY * (w / width), positionY, w, height - barWidth);

		barBgMesh.load(positionX, positionY + height - barWidth, width, barWidth, 0);
		barFgMesh.load(positionX + barY, positionY + height - barWidth, barHeight, barWidth, 0);
	}
}

void UIScrollbar::drag(float mx, float my) {
	float dx = mx - lastMx;
	float dy = my - lastMy;

	if (orientation == Orientation::VERTICAL)
		barY += dy;
	else
		barY += dx;
	calc();
}

void UIScrollbar::setRadii(float radii, bool upperLeft, bool lowerLeft, bool upperRight, bool lowerRight) {
    barBgMesh.setRadii(radii, upperLeft, lowerLeft, upperRight, lowerRight);
    barFgMesh.setRadii(radii, upperLeft, lowerLeft, upperRight, lowerRight);
}

void UIScrollbar::mousePositionInput(double mx, double my) {
	if (orientation == Orientation::VERTICAL)
		hovered = mx >= positionX + width - barWidth && my >= positionY + barY &&
		          mx <= positionX + width && my <= positionY + barY + barHeight;
	else
		hovered = mx >= positionX + barY && my >= positionY + height - barWidth &&
		          mx <= positionX + barY + barHeight && my <= positionY + height;

	if (dragging)
		drag(mx, my);

	lastMx = mx;
	lastMy = my;
}

void UIScrollbar::mouseButtonInput(int action) {
	if (action == INPUT_RELEASED)
		dragging = false;
	if (action == INPUT_PRESSED && hovered)
		dragging = true;
}

void UIScrollbar::setPosition(float positionX, float positionY) {
	this->positionX = positionX;
	this->positionY = positionY;
	calc();
}

void UIScrollbar::setSize(float width, float height) {
	this->width = width;
	this->height = height;
	calc();
}

void UIScrollbar::setBounds(float positionX, float positionY, float width, float height) {
	this->positionX = positionX;
	this->positionY = positionY;
	this->width = width;
	this->height = height;
	calc();
}