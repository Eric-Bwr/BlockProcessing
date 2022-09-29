#pragma once

#include <chrono>
#include <Network/Client/Client.h>
#include "BlockProcessing/Game/Features/Terrain/World/AsyncLoader.h"

class ServerMenuInterface;

class NetworkManager : Network::Client {
public:
    void init();
    void setServerMenuInterface(ServerMenuInterface* serverMenuInterface);
    void connect(std::string& server);
    void join(std::string &playerName);
    void disconnect();
    void update();
    void OnConnected() override;
    void OnConnectionFailed() override;
    void OnTCPPacketReceived(std::shared_ptr<Network::Packet>& packet) override;
    void OnTCPPacketSent(std::shared_ptr<Network::Packet>& packet) override;
    ~NetworkManager();
    bool connected = false;
    int status = 0, delay = 0;
private:
    ServerMenuInterface* serverMenuInterface;
    ThreadSafeQueue<NetPacketPtr&> packets;
};
