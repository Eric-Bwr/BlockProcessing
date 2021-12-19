#include "BorderLayout.h"

BorderLayout::BorderLayout(float width, float height) : Layout(width, height) {
	gapX = 10;
	gapY = 10;
}

void BorderLayout::adjust() {
	float top = insets.top;
	float bottom = height - insets.bottom;
	float left = insets.left;
	float right = width - insets.right;

	if (north != nullptr) {
		north->setBounds(left, top, right - left, north->height);
		top += north->height + gapY;
	}

	if (south != nullptr) {
		south->setBounds(left, bottom - south->height, right - left, south->height);
		bottom -= south->height + gapY;
	}

	if (east != nullptr) {
		east->setBounds(right - east->width, top, east->width, bottom - top);
		right -= east->width + gapX;
	}

	if (west != nullptr) {
		west->setBounds(left, top, west->width, bottom - top);
		left += west->width + gapX;
	}

	if (center != nullptr)
		center->setBounds(left, top, right - left, bottom - top);
}

void BorderLayout::add(UIComponent *component) {
	add(component, LayoutPosition::CENTER);
}

void BorderLayout::add(UIComponent *component, LayoutPosition pos) {
	switch (pos) {
	case LayoutPosition::CENTER:
		center = component;
		break;
	case LayoutPosition::NORTH:
		north = component;
		break;
	case LayoutPosition::SOUTH:
		south = component;
		break;
	case LayoutPosition::EAST:
		east = component;
		break;
	case LayoutPosition::WEST:
		west = component;
		break;
	}
	components.emplace_back(component);
	adjust();
}

void BorderLayout::setGapX(float gapX) {
    this->gapX = gapX;
	adjust();
}

void BorderLayout::setGapY(float gapY) {
	this->gapY = gapY;
	adjust();
}