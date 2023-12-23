#pragma once
#include <core/Packet.hpp>
#include <vector>

using int8 = char;
using int16 = short;
using int32 = int;
using int64 = long long;
using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

namespace gen {
    enum PacketId {
        CHAT
    };
    
    class Chat
            : public sv::Packet {
    public:
        Chat() : sv::Packet(PacketId::CHAT) {
        }
        ~Chat() {
    
        }
    public:
        void read() override
        {
            Packet::read();
            *this >> str;
        }
        void write() override
        {
            *this << str;
            finish();
        }
    public:
        std::string str;
    };

}
