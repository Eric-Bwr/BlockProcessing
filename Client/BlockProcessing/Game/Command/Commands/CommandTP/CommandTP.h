#pragma once

#include "BlockProcessing/Game/Command/Command.h"
#include "BlockProcessing/Game/Player/Player.h"

struct CommandTP : Command {
public:
    CommandTP(Player& player);
    void init() override;
    void execute(const std::string& typed, int length, const std::vector<std::string>& arguments) override;
private:
    Player& player;
};