#pragma once

#include <Client/Client.h>
#include <string>

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
private:
    ServerMenuInterface* serverMenuInterface;
};
