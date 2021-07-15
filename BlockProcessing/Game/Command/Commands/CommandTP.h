#pragma once

#include "../Command.h"

struct CommandTP : Command {
public:
    CommandTP();
    void execute(int length, const std::vector<std::string>& arguments) override;
};