#pragma once

#include <thread>
#include <Network/Network.h>

class NetworkManager;

class NetworkClient : public Network::Client {
public:
    void init(NetworkManager* networkManager);
    void connect(const std::string& server);
    void OnConnect(Network::TCPConnection* connection) override;
    void OnConnectFail() override;
    void OnDisconnect() override;
    void OnError(Network::TCPConnection* connection, short error) override;
    void OnPacketReceive(Network::TCPConnection* connection, Network::Packet* packet) override;
    void OnPacketSend(Network::TCPConnection* connection, Network::Packet* packet) override;
    ~NetworkClient();
};