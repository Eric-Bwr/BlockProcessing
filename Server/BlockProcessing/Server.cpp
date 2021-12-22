#include "Server.h"
#include "iostream"

#define PACKET_MOTD 0

void Server::OnConnect(Network::TCPConnection &connection) {
    auto motd = Network::Packet(PACKET_MOTD);
    motd.AppendString("A Test Server");
    connection.OutStream.push(motd);
}

void Server::OnDisconnect(Network::TCPConnection &connection, Network::DisconnectReason reason) {
    std::cout << "Disconnect: " << connection.Endpoint.GetIP() << "\n";
}

void Server::OnPacketReceive(Network::TCPConnection &connection, Network::Packet &packet) {
    std::cout << "Packet from: " << connection.Endpoint.GetIP() << "\n" <<
              "Packet: " << packet.GetString() << "\n";
}

void Server::OnPacketSend(Network::TCPConnection &connection, Network::Packet &packet) {}
