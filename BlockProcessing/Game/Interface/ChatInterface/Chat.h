#pragma once

#include "ChatInterface.h"

class Chat {
public:
    static void init();
    static void append(const std::string& text, const UIColor& textColor);
    static void revertUp();
    static void revertDown();
    static void display(bool display);
    ~Chat();
};