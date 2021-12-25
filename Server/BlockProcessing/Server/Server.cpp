#include "Server.h"
#include "../Util/Logger/Logger.h"

const int PACKET_INFO = 0;
const int PACKET_DISCONNECT = 1;
const int PACKET_PING = 2;

static Network::Packet info = Network::Packet(PACKET_INFO);

void Server::init(const char* name, const char* motd) {
    info.AppendString(motd);
    info.AppendString(name);
}

void Server::OnConnect(Network::TCPConnection &connection) {
    LOG<INFO_LVL>("New Connection: " + connection.Endpoint.GetIP());
    connection.OutStream.push(info);
}

void Server::OnDisconnect(Network::TCPConnection &connection, Network::DisconnectReason reason) {
    LOG<INFO_LVL>("Disconnected: " + connection.Endpoint.GetIP());
    if(reason == Network::DisconnectReason::InvalidPacketLength)
        LOG<WARN_LVL>("Invalid Packet Length");
    LOG<INFO_LVL>("Disconnect reason: " + std::to_string(static_cast<int>(reason)));
}

void Server::OnPacketReceive(Network::TCPConnection &connection, Network::Packet &packet) {
    switch(packet.GetPacketType()){
        case PACKET_PING:
            connection.OutStream.push(packet);
            LOG<INFO_LVL>("Ping Packet from: " + connection.Endpoint.GetIP());
            break;
        default:
            LOG<INFO_LVL>("Packet from: " + connection.Endpoint.GetIP());
            LOG<INFO_LVL>("ID: " + packet.GetPacketType());
            LOG<INFO_LVL>("Content: " + packet.GetString());
            break;
    }
}

void Server::OnPacketSend(Network::TCPConnection &connection, Network::Packet &packet) {}
