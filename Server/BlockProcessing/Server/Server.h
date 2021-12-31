#pragma once

#include <Server/Server.h>
#include "../Util/Parameters/Parameters.h"
#include "PlayerManager.h"

class Server : public Network::Server {
public:
    void init(const char* name, const char* motd);
    void OnConnect(Network::TCPConnection& connection) override;
    void OnDisconnect(Network::TCPConnection& connection, Network::DisconnectReason reason) override;
    void OnPacketReceive(Network::TCPConnection& connection, Network::Packet& packet) override;
    void OnPacketSend(Network::TCPConnection& connection, Network::Packet& packet) override;
private:
    PlayerManager playerManager;
};