#pragma once

#include "../IPEndpoint/IPEndpoint.h"
#include "../IPVersion.h"
#include "../SocketHandle.h"
#include "../SocketOption.h"
#include "../Packet/Packet.h"

#ifdef linux

#include <fcntl.h>

#endif
namespace Network {
    class NETWORK_API Socket {
    public:
        Socket(IPVersion ipVersion = IPVersion::IPv4, SocketHandle handle = INVALID_SOCKET);
        IPVersion GetIPVersion() const;
        SocketHandle GetHandle() const;
        bool Accept(Socket& socket, IPEndpoint& endpoint);
        bool Bind(const IPEndpoint& endpoint);
        bool Close();
        bool Connect(const IPEndpoint& endpoint);
        bool Create();
        bool Listen(const IPEndpoint& endpoint);
        bool SetBlocking(bool blocking);
        bool SetSocketOption(SocketOption option, BOOL value);
    private:
        IPVersion m_IPVersion = IPVersion::IPv4;
        SocketHandle m_Handle = INVALID_SOCKET;
    };
}
