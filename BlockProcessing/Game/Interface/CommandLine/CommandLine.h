#pragma once

#include "../Interface.h"

class CommandLine : Interface {
public:
    void init();
    void display(bool display) const;
    UITextField* textField;
};