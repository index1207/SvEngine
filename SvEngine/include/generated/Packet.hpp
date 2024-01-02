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
		A = 1,
		B = 2,
		C = 3,
		D = 4
    };

    class A
            : public sv::Packet {
    public:
        A() : sv::Packet(static_cast<unsigned short>(PacketId::A)) {
        }
        ~A() {
    
        }
    public:
        void read() override
        {
            Packet::read();
            *this >> var;
        }
        void write() override
        {
            *this << var;
            finish();
        }
    public:
        Int32 var;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, A& a) {
        pk >> a.var;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const A& a) {
        pk << a.var;
        return pk;
    }

	class B
            : public sv::Packet {
    public:
        B() : sv::Packet(static_cast<unsigned short>(PacketId::B)) {
        }
        ~B() {
    
        }
    public:
        void read() override
        {
            Packet::read();
            *this >> var;
        }
        void write() override
        {
            *this << var;
            finish();
        }
    public:
        Int32 var;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, B& b) {
        pk >> b.var;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const B& b) {
        pk << b.var;
        return pk;
    }

	class C
            : public sv::Packet {
    public:
        C() : sv::Packet(static_cast<unsigned short>(PacketId::C)) {
        }
        ~C() {
    
        }
    public:
        void read() override
        {
            Packet::read();
            *this >> var;
        }
        void write() override
        {
            *this << var;
            finish();
        }
    public:
        Int32 var;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, C& c) {
        pk >> c.var;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const C& c) {
        pk << c.var;
        return pk;
    }

	class D
            : public sv::Packet {
    public:
        D() : sv::Packet(static_cast<unsigned short>(PacketId::D)) {
        }
        ~D() {
    
        }
    public:
        void read() override
        {
            Packet::read();
            *this >> var;
        }
        void write() override
        {
            *this << var;
            finish();
        }
    public:
        Int32 var;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, D& d) {
        pk >> d.var;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const D& d) {
        pk << d.var;
        return pk;
    }

}
#pragma warning(pop)