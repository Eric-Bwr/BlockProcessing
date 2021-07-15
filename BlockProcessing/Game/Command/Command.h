#pragma once

#include <vector>
#include <string>
#include "../Interface/ChatInterface/ChatManager.h"

bool isNumber(const std::string& str);

struct Command {
    Command() = default;
    virtual void execute(int length, const std::vector<std::string>& arguments) {}
    const char* prefix = "";
};