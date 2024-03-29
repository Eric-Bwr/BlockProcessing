#include "CommandTP.h"
#include "BlockProcessing/Game/Engine/Interface/Messages.h"
#include "BlockProcessing/Game/Engine/Command/CommandManager.h"

static CommandManager* command;
static std::string input;

CommandTP::CommandTP(Player* player) : player(player){
    prefix = "tp";
}

void CommandTP::init() {
    command = commandManager;
}

void CommandTP::execute(const std::string& typed, int length, const std::vector<std::string>& arguments) {
    if(length == 3){
        auto x = arguments.at(0);
        auto y = arguments.at(1);
        auto z = arguments.at(2);
        if(isNumber(x) && isNumber(y) && isNumber(z)) {
            player->position.x = std::stof(x);
            player->position.y = std::stof(y);
            player->position.z = std::stof(z);
            player->update();
            std::string print = MESSAGES_SUCCESS_TP;
            print += " to " + arguments.at(0) + " " + arguments.at(1) + " " + arguments.at(2);
            auto chatComponent = new ChatComponent(print, MESSAGES_SUCCESS_COLOR);
            chatComponent->typed = typed;
            input = typed.substr(1, typed.size());
            chatComponent->callback = ([](bool hovered, bool pressed, ChatComponent* chatComponent) {
                if (hovered && pressed)
                    command->execute(input);
            });
            chatInterface->append(chatComponent);
            return;
        }
    }
    auto chatComponent = new ChatComponent(MESSAGES_ERROR_HELP_TP, MESSAGES_ERROR_COLOR);
    chatComponent->typed = typed;
    chatInterface->append(chatComponent);
}