#pragma once

#include <string>
#include <Math/Vector.h>

#define COLOR_DECIMAL 1
#define COLOR_NORMALIZED 2

class UIColor{
public:
    UIColor();
    UIColor(float red, float green, float blue, float alpha, int colorMode = COLOR_NORMALIZED);
    UIColor(std::initializer_list<float> colors, int colorMode = COLOR_NORMALIZED);
    explicit UIColor(int hex, float alpha = 1.0f);
	explicit UIColor(const std::string& hex, float alpha = 1.0f);
    Vec4f getNormalizedDecimal() const;
    Vec4f getDecimal() const;
    std::string getHexString(bool alpha = false, std::string heading = "");
    int getHex(bool alpha = false);
    void setRGBA(float red, float green, float blue, float alpha, int colorMode);
    void setRed(float red,int colorMode);
    void setGreen(float green,int colorMode);
    void setBlue(float blue,int colorMode);
    void setAlpha(float alpha,int colorMode);
    UIColor darker() const;
    UIColor brighter() const;
    ~UIColor();
    float r = 0.0f, g = 0.0f, b = 0.0f, a = 1.0f;
};

const UIColor COLOR_WHITE = UIColor(0xFFFFFF);
const UIColor COLOR_BLACK = UIColor(0x000000);
const UIColor COLOR_RED = UIColor(0xFF0000);
const UIColor COLOR_GREEN = UIColor(0x00FF00);
const UIColor COLOR_BLUE = UIColor(0x0000FF);
const UIColor COLOR_YELLOW = UIColor(0xFFFF00);
const UIColor COLOR_PURPLE = UIColor(0xFF00FF);
const UIColor COLOR_CYAN = UIColor(0x00FFFF);