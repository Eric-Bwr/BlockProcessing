#pragma once

#include <Shader.h>
#include <Math/Matrix.h>
#include <chrono>

#include "UITexture.h"
#include "Components/UIComponent.h"
#include "Components/Text/UIText.h"
#include "Components/Text/Font.h"
#include "Components/TextField/UITextField.h"
#include "Components/TextArea/UITextArea.h"
#include "Components/Button/UIButton.h"
#include "Components/Image/UIImage.h"
#include "Components/Slider/UISlider.h"
#include "Components/SplitPane/UISplitPane.h"
#include "Components/Scrollbar/UIScrollbar.h"
#include "Components/Switch/UISwitch.h"
#include "Layout/Layout.h"
#include "Layout/FlowLayout.h"
#include "Layout/BoxLayout.h"
#include "Layout/GridLayout.h"
#include "Layout/BorderLayout.h"

class UIManager {
public:
	void init(int width, int height, bool scaleOnResize = false);
	void add(UIComponent *component, int order = 0);
	void remove(UIComponent *component);
	void setSize(int width, int height);
	void keyInput(int key, int action, int mods);
	void charInput(unsigned int key);
	void mousePositionInput(double x, double y);
	void mouseButtonInput(int button, int action);
	inline int getWidth() { return width; }
	inline int getHeight() { return height; }
	void render();
    ~UIManager();
private:
    Mat4f ortho;
    Shader* textShader;
    Shader* quadShader;
    int width, height;
    double mouseX = 0, mouseY = 0;
	std::map<int, std::vector<UIComponent *>*> components;
	void renderComponent(UIComponent *);
	bool cursor = false, scaleOnResize = false;
    std::chrono::time_point<std::chrono::system_clock> start;
};