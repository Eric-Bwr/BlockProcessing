#include "CommandManager.h"
#include "../Interface/Messages.h"

void CommandManager::init(ChatInterface* chatInterface) {
    this->chatInterface = chatInterface;
}

void CommandManager::add(Command* command) {
    command->chatInterface = chatInterface;
    commands.insert(std::pair<char*, Command*>(command->prefix, command));
}

void CommandManager::execute(const std::string &input) {
    auto arguments = split(input);
    for (auto command : commands) {
        if (command.first == arguments.at(0)) {
            arguments.erase(arguments.begin());
            command.second->execute(arguments.size(), arguments);
            return;
        }
    }
    chatInterface->append(MESSAGES_ERROR_UNKNOWN_CMD, COLOR_RED);
}

CommandManager::~CommandManager() {
    for(auto command : commands)
        delete command.second;
}
