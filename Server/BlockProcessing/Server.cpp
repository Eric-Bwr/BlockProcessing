#include "Server.h"
#include "iostream"

#define PACKET_MOTD 0

void Server::OnConnect(Network::TCPConnection *connection) {
    auto motd = new Network::Packet(PACKET_MOTD);
    motd->AppendString("A Test Server");
    connection->m_Outgoing.Append(motd);
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
