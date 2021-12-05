#include "CommandManager.h"
#include "BlockProcessing/Game/Engine/Interface/Messages.h"

void CommandManager::init(ChatInterface* chatInterface) {
    this->chatInterface = chatInterface;
}

void CommandManager::add(Command* command) {
    command->commandManager = this;
    command->chatInterface = chatInterface;
    command->init();
    commands.insert(std::pair<char*, Command*>(command->prefix, command));
}

void CommandManager::execute(const std::string &input) {
    auto arguments = split(input);
    for (auto command : commands) {
        if (command.first == arguments.at(0)) {
            arguments.erase(arguments.begin());
            command.second->execute("/" + input, arguments.size(), arguments);
            return;
        }
    }
    auto chatComponent = new ChatComponent(MESSAGES_ERROR_UNKNOWN_CMD, MESSAGES_ERROR_COLOR);
    chatComponent->typed = "/" + input;
    chatInterface->append(chatComponent);
}

CommandManager::~CommandManager() {
    for(auto command : commands)
        delete command.second;
}
