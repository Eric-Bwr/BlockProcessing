#pragma once

#include <thread>
#include <Client/Client.h>

class NetworkManager;

class NetworkClient : public Network::Client {
public:
    void init(NetworkManager* networkManager);
    void connect(const std::string& server);
    void OnConnect() override;
    void OnConnectFail() override;
    void OnDisconnect() override;
    void OnPacketReceive(Network::Packet& packet) override;
    void OnPacketSend(Network::Packet& packet) override;
    ~NetworkClient();
};