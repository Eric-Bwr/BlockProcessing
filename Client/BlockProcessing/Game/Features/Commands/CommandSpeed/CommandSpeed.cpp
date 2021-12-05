#include "CommandSpeed.h"
#include "BlockProcessing/Game/Engine/Interface/Messages.h"
#include "BlockProcessing/Game/Engine/Command/CommandManager.h"

CommandSpeed::CommandSpeed(Player& player) : player(player){
    prefix = "speed";
}

void CommandSpeed::init() {}

void CommandSpeed::execute(const std::string& typed, int length, const std::vector<std::string>& arguments) {
    if(length == 1){
        auto x = arguments.at(0);
        if(isNumber(x)){
            player.speed = std::stof(x);
            std::string print = MESSAGES_SUCCESS_SPEED;
            print += x;
            auto chatComponent = new ChatComponent(print, MESSAGES_SUCCESS_COLOR);
            chatComponent->typed = typed;
            chatInterface->append(chatComponent);
            return;
        }
    }
    auto chatComponent = new ChatComponent(MESSAGES_ERROR_HELP_SPEED, MESSAGES_ERROR_COLOR);
    chatComponent->typed = typed;
    chatInterface->append(chatComponent);
}