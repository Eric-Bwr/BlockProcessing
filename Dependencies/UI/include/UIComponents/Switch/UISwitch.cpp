#include "UISwitch.h"

UISwitch::UISwitch(float width, float height)
		: UISwitch(0, 0, width, height) {}

UISwitch::UISwitch(float positionX, float positionY, float width, float height) {
	type = UIComponentType::UISWITCH;
	this->positionX = positionX;
	this->positionY = positionY;
	this->width = width;
	this->height = height;
	this->bgColor = UIColor(0xff7b5f);
	this->switchColor = UIColor(0xffe6dc);
	this->switchX = 0;
	switchSize = 1;

	bgMesh.load(positionX, positionY, width, height, 0);
	float ass = switchSize * height;
	float mm = (ass - height) / 2;
	switchMesh.load(positionX + switchX, positionY - mm, ass, ass, 0);
}

void UISwitch::setPosition(float positionX, float positionY) {
	this->positionX = positionX;
	this->positionY = positionY;
	bgMesh.load(positionX, positionY, width, height, 0);
	float ass = switchSize * height;
	float mm = (ass - height) / 2;
	switchMesh.load(positionX + switchX, positionY - mm, ass, ass, 0);
}

void UISwitch::setSize(float width, float height) {
	this->width = width;
	this->height = height;
	bgMesh.load(positionX, positionY, width, height, 0);
	float ass = switchSize * height;
	float mm = (ass - height) / 2;
	switchMesh.load(positionX + switchX, positionY - mm, ass, ass, 0);
}

void UISwitch::setBounds(float x, float y, float w, float h) {
	this->positionX = x;
	this->positionY = y;
	this->width = w;
	this->height = h;
	bgMesh.load(positionX, positionY, width, height, 0);
	float ass = switchSize * height;
	float mm = (ass - height) / 2;
	switchMesh.load(positionX + switchX, positionY - mm, ass, ass, 0);
}

void UISwitch::setCircular(bool c) {
	this->circular = c;
	updateRadius();
}

bool UISwitch::isCircular() {
	return this->circular;
}

bool UISwitch::isEnabled() {
	return this->enabled;
}

void UISwitch::setEnabled(bool e) {
	float ass = switchSize * height;
	if (e)
		switchX = width - ass;
	else
		switchX = 0;
	this->enabled = e;
	switchMesh.load(positionX + switchX, positionY, ass, ass, 0);
}

void UISwitch::mousePositionInput(double x, double y) {
	float ass = switchSize * height;
	float mm = (ass - height) / 2;
	hovered = x >= positionX + switchX && x <= positionX + switchX + ass
			&& y >= positionY - mm && y <= positionY + ass - mm;
}

void UISwitch::mouseButtonInput(int action) {
	if (hovered && action == INPUT_RELEASED)
		enabled = !enabled; /* dont use setEnabled! */
}

float UISwitch::getSwitchSize() const {
	return switchSize;
}

void UISwitch::setSwitchSize(float w) {
	switchSize = w;
	float ass = switchSize * height;
	float mm = (ass - height) / 2;
	switchMesh.load(positionX + switchX, positionY - mm, ass, ass, 0);
	updateRadius();
}

void UISwitch::setSwitchX(float x) {
	switchX = x;
	float ass = switchSize * height;
	float mm = (ass - height) / 2;
	switchMesh.load(positionX + switchX, positionY - mm, ass, ass, 0);
}

float UISwitch::getSwitchX() {
	return switchX;
}

void UISwitch::updateRadius() {
	float ass = height * switchSize;
	if (this->circular) {
		switchMesh.setRadii(ass / 2, true, true, true, true);
		bgMesh.setRadii(height / 2, true, true, true, true);
	} else {
		switchMesh.setRadii(0, false, false, false, false);
		bgMesh.setRadii(0, false, false, false, false);
	}
}
