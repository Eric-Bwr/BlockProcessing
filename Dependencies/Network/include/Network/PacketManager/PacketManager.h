#pragma once

#include <memory>
#include <queue>
#include "../Packet/Packet.h"

namespace Network {
    enum class NETWORK_API PacketManagerTask {
        ProcessPacketData, ProcessPacketSize
    };
    class NETWORK_API PacketManager {
    public:
        PacketManager() = default;
        ~PacketManager();
        bool HasPackets() const;
        Packet* Retrieve() const;
        void Append(Packet* packet);
        void Clear();
        void Pop();
    public:
        int m_CurrentPacketBufferOffset = 0;
        uint16_t m_CurrentPacketSize = 0;
        PacketManagerTask m_CurrentTask = PacketManagerTask::ProcessPacketSize;
    private:
        std::queue<Packet*> m_Packets;
    };
}