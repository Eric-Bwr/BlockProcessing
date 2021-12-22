#pragma once
#include "Socket/Socket.h"
#include "Packet/Packet.h"
#include <queue>

namespace Network{
    struct TCPConnection{
        Socket Socket;
        Endpoint Endpoint;
        uint64_t Index;
        std::queue<Packet> OutStream;
    };
}