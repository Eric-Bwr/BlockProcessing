#include "CommandTP.h"
#include "Game/Player/Player.h"
#include "../../Interface/Messages.h"

CommandTP::CommandTP(){
    prefix = "tp";
}

void CommandTP::execute(int length, const std::vector<std::string>& arguments) {
    if(length == 3){
        auto x = arguments.at(0);
        auto y = arguments.at(1);
        auto z = arguments.at(2);
        if(isNumber(x) && isNumber(y) && isNumber(z)) {
            Player::camPos.x = std::stof(x);
            Player::camPos.y = std::stof(y);
            Player::camPos.z = std::stof(z);
            Player::updatePlayer();
            return;
        }
    }
    Chat::append(MESSAGES_ERROR_HELP_TP, COLOR_RED);
}