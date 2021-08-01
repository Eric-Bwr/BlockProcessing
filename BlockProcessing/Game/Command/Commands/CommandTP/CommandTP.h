#pragma once

#include "Game/Command/Command.h"
#include "Game/Player/Player.h"

struct CommandTP : Command {
public:
    CommandTP(Player& player);
    void init() override;
    void execute(const std::string& typed, int length, const std::vector<std::string>& arguments) override;
private:
    Player& player;
};