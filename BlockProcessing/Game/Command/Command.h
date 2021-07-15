#pragma once

#include <vector>
#include <string>
#include "../Interface/ChatInterface/Chat.h"

bool isNumber(const std::string& str);

struct Command {
public:
    Command() = default;
    virtual void execute(int length, const std::vector<std::string>& arguments) {}
    const char* prefix = "";
    virtual void test() {}
};