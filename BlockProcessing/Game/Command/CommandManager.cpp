#include "CommandManager.h"
#include "Commands/CommandTP.h"

static std::vector<Command *> commands;

void CommandManager::init() {
    commands.emplace_back(new CommandTP());
}

std::vector<std::string> split(const std::string &txt) {
    std::vector<std::string> arguments;
    size_t pos = txt.find(' ');
    size_t initialPos = 0;
    while (pos != std::string::npos) {
        arguments.emplace_back(txt.substr(initialPos, pos - initialPos));
        initialPos = pos + 1;
        pos = txt.find(' ', initialPos);
    }
    arguments.emplace_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));
    return arguments;
}

#include "iostream"
void CommandManager::execute(std::string command) {
    if (!command.empty()) {
        auto arguments = split(command);
        for(auto command : commands){
            if(command->prefix == arguments.at(0)){
                arguments.erase(arguments.begin());
                command->execute(arguments);
                return;
            }
        }
    }
    std::cout << "Unknown CMD\n";
}