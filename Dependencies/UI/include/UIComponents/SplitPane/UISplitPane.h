#pragma once

#include <Texture.h>
#include "../UIComponent.h"
#include "../../UIStructure/QuadMesh.h"
#include "../../UIColor.h"

class UISplitPane: public UIComponent {
public:
	explicit UISplitPane(float positionX = 0, float positionY = 0, float width = DEFAULT_WIDTH, float height = DEFAULT_HEIGHT, UIComponent *left = nullptr, UIComponent *right = nullptr, Orientation o = Orientation::HORIZONTAL);
	explicit UISplitPane(UIComponent *left = nullptr, UIComponent *right = nullptr, Orientation o = Orientation::HORIZONTAL);

    void setDividerColor(const UIColor& standardColor, const UIColor& hoverColor, const UIColor& pressedColor);
    void setDividerTexture(Texture* texture, float buttonX, float buttonY, float buttonWidth, float buttonHeight);
    void setDividerTexture(Texture* texture, float buttonX, float buttonY, float buttonWidth, float buttonHeight, float hoverX, float hoverY, float hoverWidth, float hoverHeight, float pressedX, float pressedY, float pressedWidth, float pressedHeight);
    void setPosition(float positionX, float positionY) override;
    void setBounds(float positionX, float positionY, float width, float height) override;
    void setSize(float width, float height);
	void setLeft(UIComponent * left);
	void setRight(UIComponent *right);
	void setOrientation(Orientation orientation);
	void setDivider(float divider);

	inline UIComponent *getLeft(){ return left; }
	inline UIComponent *getRight(){ return right; }
	inline Orientation getOrientation(){ return orientation; }
	inline float getDivider() const { return divider; }

    void mousePositionInput(double x, double y) override;
    void mouseButtonInput(int action) override;
    void keyInput(int key, int action, int mods) override;
    void charInput(unsigned int key) override;

    QuadMeshTriplet mesh;
	UIColorTriplet dividerColor;
	Texture* texture;
    bool dragging, hovered;
private:
	void calc();
	Orientation orientation;
	UIComponent *left;
	UIComponent *right;
	float divider, divWidth;
    int mode = 0;
};