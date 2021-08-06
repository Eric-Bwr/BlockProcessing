#pragma once
#define WIN32_LEAN_AND_MEAN

#include <cstdint>
#include <string>
#include <vector>
#include "../Core/Core.h"

#ifdef _WIN32

#include <winsock2.h>

#endif
#ifdef linux

#include <sys/socket.h>
#include <arpa/inet.h>

#endif
#define NETWORK_MAX_PACKET_SIZE 4080
namespace Network {
    class NETWORK_API Packet {
    public:
        explicit Packet(uint16_t packetType = 0, bool encrypted = false);
        void Clear();
        void SetPacketType(uint16_t packetType);
        uint16_t GetPacketType() const;
        void SetEncrypted(bool encrypted);
        bool IsEncrypted() const;
        void Append(const void* data, uint32_t size);
        void AppendUint16(uint16_t data);
        void AppendUint32(uint32_t data);
        void AppendUint64(uint64_t data);
        void AppendString(const std::string& data);
        void AppendBool(bool data);
        uint16_t GetUint16();
        uint32_t GetUint32();
        uint64_t GetUint64();
        std::string GetString();
        bool GetBool();
    public:
        std::vector<char> m_Buffer = {};
        uint32_t m_BufferOffset = 0;
    };
}