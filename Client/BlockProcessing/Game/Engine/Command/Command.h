#pragma once

#include <vector>
#include <string>
#include "BlockProcessing/Game/Features/Interfaces/GameInterfaces/ChatInterface/ChatInterface.h"

bool isNumber(const std::string& str);
std::vector<std::string> split(const std::string &text);

struct Command {
public:
    virtual void init() {}
    virtual void execute(const std::string& typed, int length, const std::vector<std::string>& arguments) {}
    CommandManager* commandManager;
    ChatInterface* chatInterface;
    const char* prefix = "";
};
