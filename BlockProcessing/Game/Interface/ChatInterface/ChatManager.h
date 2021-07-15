#pragma once

#include <map>
#include "ChatComponent.h"

class ChatManager {
public:
    static void init();
    static void append(ChatComponent component);
};