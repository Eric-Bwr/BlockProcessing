#pragma once

#include "Command.h"

class CommandManager {
public:
    static void init();
    static void execute(const std::string& command);
};