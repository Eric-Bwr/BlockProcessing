#include "Server.h"
#include "../Util/Logger/Logger.h"

const int PACKET_INFO = 0;

void Server::OnConnect(Network::TCPConnection &connection) {
    LOG<INFO_LVL>("New Connection: " + connection.Endpoint.GetIP());
    auto motd = Network::Packet(PACKET_INFO);
    motd.AppendString("A Test Server");
    motd.AppendString("Test Motd");
    connection.OutStream.push(motd);
}

void Server::OnDisconnect(Network::TCPConnection &connection, Network::DisconnectReason reason) {
    LOG<INFO_LVL>("Disconnected: " + connection.Endpoint.GetIP());
}

void Server::OnPacketReceive(Network::TCPConnection &connection, Network::Packet &packet) {
    LOG<INFO_LVL>("Packet from: " + connection.Endpoint.GetIP());
    LOG<INFO_LVL>("Content: " + packet.GetString());
}

void Server::OnPacketSend(Network::TCPConnection &connection, Network::Packet &packet) {}
