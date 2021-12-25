#include "Client.h"
#include <iostream>

bool Network::Client::Connect(const Network::Endpoint &endpoint) {
    Socket socket = Socket();
    if (!socket.Create()) {
        return false;
    }
    if (!socket.Connect(endpoint)) {
        OnConnectFail();
        return false;
    }
    if (!socket.SetBlocking(false)) {
        return false;
    }
    Connection.Socket = socket;
    Connection.Endpoint = endpoint;
    m_MasterFD.fd = Connection.Socket.GetHandle();
    m_MasterFD.events = POLLRDNORM | POLLWRNORM;
    m_MasterFD.revents = 0;
    m_ConnectionFailed = false;
    OnConnect();
    return true;
}

void Network::Client::Close() {
    Connection.Socket.Close();
    OnDisconnect();
    m_ConnectionFailed = true;
    std::queue<Network::Packet> empty;
    std::swap(Connection.OutStream, empty);
}

void Network::Client::Frame() {
    if (m_ConnectionFailed) {
        return;
    }
    Poll(&m_MasterFD, 1, 0);
    if (m_MasterFD.revents & POLLERR) {
        std::cout << "ERRRRRR 1" << std::flush;

        Close();
        return;
    }
    if (m_MasterFD.revents & POLLHUP) {
        std::cout << "ERRRRRR 2" << std::flush;

        Close();
        return;
    }
    if (m_MasterFD.revents & POLLNVAL) {
        std::cout << "ERRRRRR 3" << std::flush;
        Close();
        return;
    }
    if (m_MasterFD.revents & POLLRDNORM) {
        uint32_t size = 0;
        Connection.Socket.ReceiveAll(&size, sizeof(uint32_t));
        if (size > MAX_PACKET_SIZE) {
            std::cout << "BIGGER" << std::flush;
           // Close();
            return;
        }
        if (size < 2) {
            std::cout << "LESS" << std::flush;
           // Close();
            return;
        }
        Packet packet;
        packet.Buffer.resize(size);
        Connection.Socket.ReceiveAll(&packet.Buffer[0], size);
        OnPacketReceive(packet);
    }
    if (m_MasterFD.revents & POLLWRNORM) {
        while (!Connection.OutStream.empty()) {
            Packet &packet = Connection.OutStream.front();
            OnPacketSend(packet);
            if (!packet.IsEmpty()) {
                uint32_t size = packet.Buffer.size();
                Connection.Socket.SendAll(&size, sizeof(uint32_t));
                Connection.Socket.SendAll(packet.Buffer.data(), packet.Buffer.size());
            }
            Connection.OutStream.pop();
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds (1));
}

