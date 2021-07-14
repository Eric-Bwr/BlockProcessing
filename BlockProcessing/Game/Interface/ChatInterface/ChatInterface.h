#pragma once

#include "../Interface.h"

class ChatInterface : Interface {
public:
    void init();
    void display(bool display);
    void enter() const;
    UITextField* textField;
};