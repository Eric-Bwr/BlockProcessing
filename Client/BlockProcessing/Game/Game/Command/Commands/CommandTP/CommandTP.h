#pragma once

#include "BlockProcessing/Game/Game/Command/Command.h"
#include "BlockProcessing/Game/Game/Entity/Entities/Player/Player.h"

struct CommandTP : Command {
public:
    explicit CommandTP(Player& player);
    void init() override;
    void execute(const std::string& typed, int length, const std::vector<std::string>& arguments) override;
private:
    Player& player;
};