#pragma once

#include <vector>
#include <string>

struct Command {
    Command() = default;
    virtual void execute(const std::vector<std::string>& arguments) {}
    const char* prefix = "";
};