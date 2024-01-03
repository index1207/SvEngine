#pragma once
#pragma warning(push)
#pragma warning(disable: 26495)
#include <core/Packet.hpp>

#include <vector>

using Int8 = char;
using Int16 = short;
using Int32 = int;
using Int64 = long long;
using Uint8 = unsigned char;
using Uint16 = unsigned short;
using Uint32 = unsigned int;
using Uint64 = unsigned long long;

namespace gen {
    enum class PacketId {
		CHAT = 1
    };

    class Chat
            : public sv::Packet {
    public:
        Chat() : sv::Packet(static_cast<unsigned short>(PacketId::CHAT)) {
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
    
    inline sv::Packet& operator>>(sv::Packet& pk, Chat& chat) {
        pk >> chat.str;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const Chat& chat) {
        pk << chat.str;
        return pk;
    }

}
#pragma warning(pop)
