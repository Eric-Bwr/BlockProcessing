#pragma once

#include <string>
#include <vector>
#ifdef _WIN32
#include <ws2tcpip.h>
#endif
#ifdef linux
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif
#include "../IPVersion.h"

namespace Network {
    class IPEndpoint {
    public:
        IPEndpoint() = default;
        IPEndpoint(const char* ip, unsigned short port);
        explicit IPEndpoint(sockaddr* addr);
        const std::vector<uint8_t>& GetIPBytes() const;
        sockaddr_in GetSockAddrIPv4() const;
        const std::string& GetHostname() const;
        const std::string GetIP() const;
        IPVersion GetIPVersion() const;
        unsigned short GetPort() const;
        sockaddr_in6 GetSockAddrIPv6() const;
    private:
        std::string m_Hostname = "";
        std::vector<uint8_t> m_IPBytes;
        std::string m_IPString = "";
        IPVersion m_IPVersion = IPVersion::Unknown;
        unsigned short m_Port = 0;
    };
}