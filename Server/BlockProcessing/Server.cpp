#include "Server.h"
#include "iostream"

void Server::OnConnect(Network::TCPConnection *connection) {
    std::cout << "Connect: " << connection->GetIPEndpoint().GetIP() << "\n";
}

void Server::OnDisconnect(Network::TCPConnection *connection) {
    std::cout << "Disconnect: " << connection->GetIPEndpoint().GetIP() << "\n";
}

void Server::OnError(Network::TCPConnection *connection, short error) {}

void Server::OnPacketReceive(Network::TCPConnection *connection, Network::Packet *packet) {
    std::cout << "Packet from: " << connection->GetIPEndpoint().GetIP() << "\n" <<
              "Packet: " << packet->GetString() << "\n";
    delete packet;
}

void Server::OnPacketSend(Network::TCPConnection *connection, Network::Packet *packet) {

}
