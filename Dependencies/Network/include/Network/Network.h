#pragma once

#include "Server/Server.h"
#include "Client/Client.h"

namespace Network {
    inline  bool Initialize() {
#ifdef _WIN32
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            return false;
        }
        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
            return false;
        }
        return true;
#endif
#ifdef linux
        return true;
#endif
    }
    inline void Terminate() {
#ifdef _WIN32
        WSACleanup();
#endif
    }
}