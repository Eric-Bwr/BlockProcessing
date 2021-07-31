#pragma once

#include <vector>
#include <string>
#include "../Interface/Interfaces/ChatInterface/ChatInterface.h"

bool isNumber(const std::string& str);
std::vector<std::string> split(const std::string &text);

struct Command {
public:
    Command() = default;
    virtual void execute(int length, const std::vector<std::string>& arguments) {}
    ChatInterface* chatInterface;
    char* prefix = "";
};