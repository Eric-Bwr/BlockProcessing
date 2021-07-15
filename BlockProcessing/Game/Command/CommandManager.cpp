#include "CommandManager.h"
#include "Commands/CommandTP.h"
#include "../Interface/Messages.h"

static std::vector<Command*> commands;

void CommandManager::init() {
    commands.emplace_back(new CommandTP());
}

std::vector<std::string> split(const std::string &text) {
    std::vector<std::string> arguments;
    size_t pos = text.find(' ');
    size_t initialPos = 0;
    while (pos != std::string::npos) {
        arguments.emplace_back(text.substr(initialPos, pos - initialPos));
        initialPos = pos + 1;
        pos = text.find(' ', initialPos);
    }
    arguments.emplace_back(text.substr(initialPos, std::min(pos, text.size()) - initialPos + 1));
    return arguments;
}

void CommandManager::execute(const std::string& input) {
    if (!input.empty()) {
        auto arguments = split(input);
        for(auto command : commands){
            if(command->prefix == arguments.at(0)){
                arguments.erase(arguments.begin());
                command->execute(arguments.size(), arguments);
                return;
            }
        }
    }
    Chat::append(MESSAGES_ERROR_UNKNOWN_CMD, COLOR_RED);
}