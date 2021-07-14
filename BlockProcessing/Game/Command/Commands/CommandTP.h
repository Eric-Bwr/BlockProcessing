#pragma once

#include "../Command.h"

struct CommandTP : Command {
    CommandTP();
    void execute(const std::vector<std::string>& arguments) override;
};