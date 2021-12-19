#include "BoxLayout.h"

BoxLayout::BoxLayout(float width, float height, LayoutDirection direction) : Layout(width, height) {
	this->direction = direction;
	gap = 10;
}

void BoxLayout::adjust() {
	if (direction == LayoutDirection::ROW) {
		float off = insets.top;
		for (auto comp : components) {
			comp->setPosition(width / 2 - comp->width / 2, off);
			off += comp->height + gap;
		}
	} else if (direction == LayoutDirection::COLUMN) {
		float off = insets.left;
		for (auto comp : components) {
			comp->setPosition(off, height / 2 - comp->height / 2);
			off += comp->width + gap;
		}
	}
}

void BoxLayout::setGap(float gap) {
    this->gap = gap;
	adjust();
}

void BoxLayout::setDirection(LayoutDirection direction) {
    this->direction = direction;
	adjust();
}