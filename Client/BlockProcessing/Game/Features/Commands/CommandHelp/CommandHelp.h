#pragma once

#include "BlockProcessing/Game/Engine/Command/Command.h"

struct CommandHelp : Command {
public:
    explicit CommandHelp();
    void init() override;
    void execute(const std::string& typed, int length, const std::vector<std::string>& arguments) override;
private:
    void addHelpEntry(char* text, char* typed);
};