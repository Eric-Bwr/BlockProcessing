#include "CommandTP.h"
#include "Game/Interface/Messages.h"

CommandTP::CommandTP(Player& player) : player(player){
    prefix = "tp";
}

void CommandTP::execute(const std::string& typed, int length, const std::vector<std::string>& arguments) {
    if(length == 3){
        auto x = arguments.at(0);
        auto y = arguments.at(1);
        auto z = arguments.at(2);
        if(isNumber(x) && isNumber(y) && isNumber(z)) {
            player.camPos.x = std::stof(x);
            player.camPos.y = std::stof(y);
            player.camPos.z = std::stof(z);
            player.update();
            return;
        }
    }
    auto chatComponent = new ChatComponent(MESSAGES_ERROR_HELP_TP, MESSAGES_ERROR_COLOR);
    chatComponent->typed = typed;
    chatInterface->append(chatComponent);
}