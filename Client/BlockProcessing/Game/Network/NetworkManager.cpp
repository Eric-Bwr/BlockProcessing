#include "NetworkManager.h"
#include "iostream"

static std::atomic_bool alive = true;

static void frame(Network::Client* client){
    while(alive){
        std::this_thread::sleep_for(std::chrono::milliseconds((long) (1)));
        client->Frame();
    }
}

void NetworkManager::init() {
    Network::Initialize();
    Connect(Network::IPEndpoint("127.0.0.1", 25565));
    std::thread thread(frame, this);
    thread.detach();
}

void NetworkManager::OnConnect(Network::TCPConnection *connection) {
    std::cout << "Connect: " << connection->GetIPEndpoint().GetIP() << "\n";
    auto packet = new Network::Packet(0);
    packet->AppendString("Test");
    connection->m_Outgoing.Append(packet);
}

void NetworkManager::OnConnectFail() {
    std::cout << "Failed to connect\n";
}

void NetworkManager::OnDisconnect() {
    std::cout << "Disconnected\n";
}

void NetworkManager::OnError(Network::TCPConnection *connection, short error) {}

void NetworkManager::OnPacketReceive(Network::TCPConnection *connection, Network::Packet *packet) {
    std::cout << "Connection: " << connection->GetIPEndpoint().GetIP() << "\n" <<
              "Packet: " << packet->GetString() << "\n";
}

void NetworkManager::OnPacketSend(Network::TCPConnection *connection, Network::Packet *packet) {

}

NetworkManager::~NetworkManager() {
    alive = false;
}
