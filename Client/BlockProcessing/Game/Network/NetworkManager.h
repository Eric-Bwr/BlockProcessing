#pragma once

#include <Network/Network.h>
#include <thread>

class NetworkManager : public Network::Client {
public:
    void init();
    void OnConnect(Network::TCPConnection* connection) override;
    void OnConnectFail() override;
    void OnDisconnect() override;
    void OnError(Network::TCPConnection* connection, short error) override;
    void OnPacketReceive(Network::TCPConnection* connection, Network::Packet* packet) override;
    void OnPacketSend(Network::TCPConnection* connection, Network::Packet* packet) override;
    ~NetworkManager();
};
