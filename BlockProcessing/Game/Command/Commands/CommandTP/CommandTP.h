#pragma once

#include "Game/Command/Command.h"
#include "Game/Player/Player.h"

struct CommandTP : Command {
public:
    explicit CommandTP(Player& player);
    void execute(int length, const std::vector<std::string>& arguments) override;
private:
    Player& player;
};