#pragma once

#include <Math/Vector.h>

#define COMPONENT_HOVERED(mx, my) (mx >= positionX && my >= positionY && \
                                   mx <= positionX + width && \
                                   my <= positionY + height)

enum class Orientation {
	VERTICAL = 0,
	HORIZONTAL
};

enum UIComponentType {
	UIIMAGE = 0,
	UIBUTTON,
	UISLIDER,
	UITEXT,
    UITEXTFIELD,
    UITEXTAREA,
	UISPLITPANE,
	UISCROLLBAR,
	UILAYOUT,
	UISWITCH
};

class UIComponent {
public:
	int type;

	float getX() const { return positionX; }
	float getY() const { return positionY; }
	float getWidth() const { return width; }
	float getHeight() const { return height; }

	Vec2f getPosition() const { return Vec2f(positionX, positionY); }
	Vec2f getSize() const { return Vec2f(width, height); }

	virtual void setPosition(float positionX, float positionY) {}
	virtual void setBounds(float positionX, float positionY, float width, float height) {}

    virtual void keyInput(int key, int action, int mods) {}
    virtual void charInput(unsigned int key) {}
    virtual void mousePositionInput(double x, double y) {}
    virtual void mouseButtonInput(int action) {}

	float positionX, positionY;
	float width, height;
};