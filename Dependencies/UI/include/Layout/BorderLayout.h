#pragma once

#include "Layout.h"

class BorderLayout : public Layout {
public:
	BorderLayout(float width, float height);

	void adjust() override;

	void add(UIComponent *component) override;
	void add(UIComponent *component, LayoutPosition position) override;

    void setGapX(float gapX);
    void setGapY(float gapY);

    float getGapX() { return gapX ;}
    float getGapY() { return gapY ;}

    float gapX;
    float gapY;
private:
	UIComponent *east = nullptr;
	UIComponent *north = nullptr;
	UIComponent *west = nullptr;
	UIComponent *south = nullptr;
	UIComponent *center = nullptr;
};