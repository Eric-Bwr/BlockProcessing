#pragma once

#include <chrono>
#include <Client/Client.h>
#include "BlockProcessing/Game/Features/Terrain/World/AsyncLoader.h"

class ServerMenuInterface;

class NetworkManager : Network::Client {
public:
    void init();
    void setServerMenuInterface(ServerMenuInterface* serverMenuInterface);
    void connect(const std::string& server);
    void update();
    void OnConnect() override;
    void OnConnectFail() override;
    void OnDisconnect() override;
    void OnPacketReceive(Network::Packet& packet) override;
    void OnPacketSend(Network::Packet& packet) override;
    ~NetworkManager();
    bool connected = false;
    int status = 0;
private:
    ServerMenuInterface* serverMenuInterface;
    ThreadSafeQueue<Network::Packet> packets;
};
