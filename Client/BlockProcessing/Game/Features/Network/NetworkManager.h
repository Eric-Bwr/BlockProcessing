#pragma once

#include "NetworkClient.h"

class ServerMenuInterface;

#define ACTION_STATUS 0
#define PACKET_MOTD 0

struct NetworkAction {
    uint8_t type;
    Network::TCPConnection *connection;
    Network::Packet *packet;
    ~NetworkAction() {
        delete connection;
        delete packet;
    }
};

class NetworkManager {
public:
    void init();
    void setServerMenuInterface(ServerMenuInterface* serverMenuInterface);
    void connect(const std::string& server);
    void update();
    ~NetworkManager();
    bool connected = false;
    std::vector<NetworkAction> actions;
private:
    NetworkClient networkClient;
    ServerMenuInterface* serverMenuInterface;
};
