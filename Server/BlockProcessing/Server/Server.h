#pragma once

#include <Network/Server/Server.h>
#include "../Util/Parameters/Parameters.h"
#include "PlayerManager.h"

class Server : public Network::Server {
public:
    void init(const char* name, const char* motd);
    void OnClientConnected(NetServerClientPtr& client) override;
    void OnClientDisconnected(NetServerClientPtr& client) override;
    void OnTCPPacketReceived(NetServerClientPtr& client, NetPacketPtr& packet) override;
    void OnTCPPacketSent(NetServerClientPtr& client, NetPacketPtr& packet) override;
private:
    PlayerManager playerManager;
};