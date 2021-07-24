#pragma once

#include "../Interface.h"

class ChatInterface : Interface {
public:
    static void init();
    static void display(bool display);
    static void enter();
    static UITextField* textField;
    ~ChatInterface();
    static const int width = 1000, height = 40;
};