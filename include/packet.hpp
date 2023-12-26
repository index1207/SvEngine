#pragma once
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
    enum PacketId {
		TEST = 1,
		CHAT = 2
    };
    
    class Test
            : public sv::Packet {
    public:
        Test() : sv::Packet(PacketId::TEST) {
        }
        ~Test() {
    
        }
    public:
        void read() override
        {
            Packet::read();
            *this >> data;
        }
        void write() override
        {
            *this << data;
            finish();
        }
    public:
        Int16 data;
    };
    
    sv::Packet& operator>>(sv::Packet& pk, Test& test) {
        pk >> test.data;
        return pk;
    }

    sv::Packet& operator<<(sv::Packet& pk, const Test& test) {
        pk << test.data;
        return pk;
    }

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
            *this >> val;
        }
        void write() override
        {
            *this << val;
            finish();
        }
    public:
        std::vector<Test> val;
    };
    
    sv::Packet& operator>>(sv::Packet& pk, Chat& chat) {
        pk >> chat.val;
        return pk;
    }

    sv::Packet& operator<<(sv::Packet& pk, const Chat& chat) {
        pk << chat.val;
        return pk;
    }

}
