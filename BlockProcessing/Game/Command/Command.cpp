#include "Command.h"

bool isNumber(const std::string &str) {
    for (char const &c : str) {
        if (!std::isdigit(c)) return false;
    }
    return true;
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