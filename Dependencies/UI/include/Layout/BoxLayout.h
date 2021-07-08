#pragma once

#include "Layout.h"

class BoxLayout : public Layout {
public:

    BoxLayout(float, float, LayoutDirection dir = LayoutDirection::ROW);

    void adjust() override;

    void setGap(float gap);
    void setDirection(LayoutDirection direction);

    float getGap() { return gap; }
    LayoutDirection getDirection() { return direction; }

    float gap;
    LayoutDirection direction;
};