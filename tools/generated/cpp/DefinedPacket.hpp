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
		TEST = 1
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
            *this >> data1 >> data2 >> data3;
        }
        void write() override
        {
            *this << data1 << data2 << data3;
            finish();
        }
        void onReceive() override
        {
            PacketHandler::onReceivePacket(TEST, this);
        }
    public:
        Int32 data1;
		std::string data2;
		std::vector<std::string> data3;
	
    };
    
    sv::Packet& operator>>(sv::Packet& pk, Test& test) {
        pk >> test.data1 >> test.data2 >> test.data3;
        return pk;
    }

    sv::Packet& operator<<(sv::Packet& pk, const Test& test) {
        pk << test.data1 << test.data2 << test.data3;
        return pk;
    }

}
