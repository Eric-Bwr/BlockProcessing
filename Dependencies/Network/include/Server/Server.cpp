#include "Server.h"
#include <thread>
#include <iostream>

bool Network::Server::Initialize(const Network::Endpoint &endpoint) {
    m_MasterFD.clear();
    m_Connections.clear();
    m_Server = Socket();
    if (!m_Server.Create()) {
        return false;
    }
    if (!m_Server.SetBlocking(false)) {
        return false;
    }
    if (!m_Server.Listen(endpoint)) {
        return false;
    }
    PollFD serverFD = {};
    serverFD.fd = m_Server.GetHandle();
    serverFD.events = POLLRDNORM | POLLWRNORM;
    serverFD.revents = 0;
    m_MasterFD.push_back(serverFD);
    m_Init = true;
    return true;
}

void Network::Server::Terminate() {
    m_MasterFD.clear();
    m_Connections.clear();
    m_Server.Close();
}

void Network::Server::Frame() {
    if(!m_Init){
        return;
    }
    Poll(m_MasterFD.data(), m_MasterFD.size(), 0);
    for (int i = m_MasterFD.size() - 1; i >= 1; i--) {

        int index = i - 1;
        TCPConnection &connection = m_Connections[index];
        if (m_MasterFD[i].revents & POLLERR) {
            CloseConnection(index);
            OnDisconnect(connection, DisconnectReason::Error);
            continue;
        }
        if (m_MasterFD[i].revents & POLLHUP) {
            CloseConnection(index);
            OnDisconnect(connection, DisconnectReason::Aborted);
            continue;
        }
        if (m_MasterFD[i].revents & POLLNVAL) {
            CloseConnection(index);
            OnDisconnect(connection, DisconnectReason::InvalidSocket);
            continue;
        }
        if (m_MasterFD[i].revents & POLLRDNORM) {
            uint32_t size = 0;
            connection.Socket.ReceiveAll(&size, sizeof(uint32_t));
            if (size == 0 || size > MAX_PACKET_SIZE) {
                CloseConnection(index);
                OnDisconnect(connection, DisconnectReason::InvalidPacketLength);
                continue;
            }
            Packet packet;
            packet.Buffer.resize(size);
            connection.Socket.ReceiveAll(&packet.Buffer[0], size);
            OnPacketReceive(connection, packet);
            continue;
        }
        if (m_MasterFD[i].revents & POLLWRNORM) {
            while (!connection.OutStream.empty()) {
                Packet &packet = connection.OutStream.front();
                if (!packet.IsEmpty()) {
                    OnPacketSend(connection, packet);
                    uint32_t size = packet.Buffer.size();
                    connection.Socket.SendAll(&size, sizeof(uint32_t));
                    connection.Socket.SendAll(packet.Buffer.data(), packet.Buffer.size());
                }
                connection.OutStream.pop();
            }
            continue;
        }
    }

    if (m_MasterFD[0].revents & POLLRDNORM) {
        Socket clientSocket;
        Endpoint clientEndpoint;
        if (!m_Server.Accept(clientSocket, clientEndpoint)) {
            return;
        }
        TCPConnection connection;
        connection.Socket = clientSocket;
        connection.Endpoint = clientEndpoint;
        connection.Index = clientSocket.GetHandle();
        OnConnect(connection);
        m_Connections.emplace_back(connection);
        PollFD clientFD = {};
        clientFD.fd = clientSocket.GetHandle();
        clientFD.events = POLLRDNORM | POLLWRNORM;
        clientFD.revents = 0;
        m_MasterFD.push_back(clientFD);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds((long) (1)));
}

void Network::Server::CloseConnection(int index) {
    TCPConnection &connection = m_Connections.at(index);
    m_MasterFD.erase(m_MasterFD.begin() + (index + 1));
    connection.Socket.Close();
    m_Connections.erase(m_Connections.begin() + index);
}

