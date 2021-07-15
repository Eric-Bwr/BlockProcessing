#pragma once

#include <vector>
#include "../UIComponents/UIComponent.h"

enum class LayoutDirection {
	ROW = 0,
	COLUMN
};

enum class LayoutPosition {
	NORTH = 0,
	SOUTH = 1,
	EAST = 2,
	WEST = 3,
	CENTER = 4
};

class Insets {
public:
	Insets();
	explicit Insets(float value);
	Insets(float left, float right, float top, float bottom);

    float left;
    float right;
    float top;
    float bottom;
};

class Layout : public UIComponent {
public:
	Layout(float width, float height);

	virtual void adjust() {}

	virtual void add(UIComponent *component);
	virtual void add(UIComponent *component, LayoutPosition position);
    void setInsets(Insets insets);

	Insets getInsets();

    void keyInput(int key, int action, int mods) override;
    void charInput(unsigned int key) override;
    void mousePositionInput(double x, double y) override;
    void mouseButtonInput(int action) override;

    std::vector<UIComponent *> components = {};
protected:
	Insets insets;
};