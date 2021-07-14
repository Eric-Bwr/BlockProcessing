#include "CommandTP.h"
#include "Game/Player/Player.h"

CommandTP::CommandTP(){
    prefix = "tp";
}
#include "iostream"

void CommandTP::execute(const std::vector<std::string> &arguments) {
    std::cout << "Teleport\n";
    for(auto arg : arguments){
        std::cout << arg << " ";
    }
    std::cout << "\n";
}
