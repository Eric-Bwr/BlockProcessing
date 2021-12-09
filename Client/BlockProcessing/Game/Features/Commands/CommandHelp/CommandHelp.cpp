#include "CommandHelp.h"
#include "BlockProcessing/Game/Engine/Interface/Messages.h"
#include "BlockProcessing/Game/Engine/Command/CommandManager.h"

static ChatInterface* chat;

CommandHelp::CommandHelp(){
        prefix = "help";
}

void CommandHelp::init() {
    chat = chatInterface;
}

void CommandHelp::execute(const std::string& typed, int length, const std::vector<std::string>& arguments) {
    auto chatComponent = new ChatComponent(MESSAGES_SUCCESS_HELP, MESSAGES_DEFAULT_COLOR);
    chatComponent->typed = typed;
    chatInterface->append(chatComponent);
    addHelpEntry("/tp x y z - Teleports the player to position", "/tp");
    addHelpEntry("/speed x - Sets the players speed", "/speed");
}

void CommandHelp::addHelpEntry(char* text, char* typed){
    auto chatComponent = new ChatComponent(text, MESSAGES_DEFAULT_COLOR);
    chatComponent->typed = typed;
    chatComponent->callback = ([](bool hovered, bool pressed, ChatComponent* chatComponent) {
        if (hovered && pressed)
            chat->textField->setText(chatComponent->typed.c_str());
    });
    chatInterface->append(chatComponent);
}