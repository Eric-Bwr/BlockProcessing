#pragma once

#include <string>
#include "Command.h"

class CommandManager {
public:
    static void init();
    static void execute(std::string command);
};