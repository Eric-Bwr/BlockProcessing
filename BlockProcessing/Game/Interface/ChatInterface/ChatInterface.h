#pragma once

#include "../Interface.h"

class ChatInterface : Interface {
public:
    static void init();
    static void display(bool display);
    static void enter();
    static UITextField* textField;
};