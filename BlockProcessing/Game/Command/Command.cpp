#include "Command.h"

bool isNumber(const std::string &str) {
    for (char const &c : str) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}