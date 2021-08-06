#pragma once

#include <Network/Network.h>

class Server : public Network::Server {
public:
    Server() = default;
    void OnConnect(Network::TCPConnection* connection) override;
    void OnDisconnect(Network::TCPConnection* connection) override;
    void OnError(Network::TCPConnection* connection, short error) override;
    void OnPacketReceive(Network::TCPConnection* connection, Network::Packet* packet) override;
    void OnPacketSend(Network::TCPConnection* connection, Network::Packet* packet) override;
};