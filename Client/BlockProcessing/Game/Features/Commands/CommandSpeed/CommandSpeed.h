#pragma once

#include "BlockProcessing/Game/Engine/Command/Command.h"
#include "BlockProcessing/Game/Features/Entities/Player/Player.h"

struct CommandSpeed : Command {
public:
    explicit CommandSpeed(Player* player);
    void init() override;
    void execute(const std::string& typed, int length, const std::vector<std::string>& arguments) override;
private:
    Player* player;
};