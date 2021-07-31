#pragma once

#include "Command.h"

class CommandManager {
public:
    void init(ChatInterface* chatInterface);
    void add(Command* command);
    void execute(const std::string& command);
    ~CommandManager();
private:
    ChatInterface* chatInterface;
    std::map<char*, Command*> commands;
};