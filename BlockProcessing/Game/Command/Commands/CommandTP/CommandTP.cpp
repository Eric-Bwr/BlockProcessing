#include "CommandTP.h"
#include "Game/Interface/Messages.h"
#include "../../CommandManager.h"

static CommandManager* command;
static std::string input;

CommandTP::CommandTP(Player& player) : player(player){
    prefix = "tp";
}

void CommandTP::init() {
    command = commandManager;
}

static void teleportCallback(bool hovered, bool pressed){
    if(hovered && pressed)
        command->execute(input);
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
            std::string print = MESSAGES_SUCCESS_TP;
            print += " to " + arguments.at(0) + " " + arguments.at(1) + " " + arguments.at(2);
            auto chatComponent = new ChatComponent(print, MESSAGES_SUCCESS_COLOR);
            chatComponent->typed = typed;
            input = typed.substr(1, typed.size());
            chatComponent->callback = teleportCallback;
            chatInterface->append(chatComponent);
            return;
        }
    }
    auto chatComponent = new ChatComponent(MESSAGES_ERROR_HELP_TP, MESSAGES_ERROR_COLOR);
    chatComponent->typed = typed;
    chatInterface->append(chatComponent);
}