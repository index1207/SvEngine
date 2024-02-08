#pragma once
#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 4100)
#include "Packet.gen.hpp"

#ifdef __UNREAL__
#include "Network/Packet.h"
#elif __SERVER__
#include "core/Packet.hpp"
#include "util/Types.hpp"
#endif

#include <vector>

/* Additional headers. */
#include "Enum.gen.hpp"

namespace gen {
    class Vec3
            : public sv::Packet {
    public:
        Vec3() : sv::Packet(static_cast<unsigned short>(PacketId::None)) {
        }
        ~Vec3() {
    
        }
    protected:
        void read() override
        {
            Packet::read();
            *this >> x >> y >> z;
        }
        void write() override
        {
            *this << x << y << z;
            finish();
        }
    public:
        float x;
		float y;
		float z;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, Vec3& vec3) {
        pk >> vec3.x >> vec3.y >> vec3.z;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const Vec3& vec3) {
        pk << vec3.x << vec3.y << vec3.z;
        return pk;
    }

	class Status
            : public sv::Packet {
    public:
        Status() : sv::Packet(static_cast<unsigned short>(PacketId::None)) {
        }
        ~Status() {
    
        }
    protected:
        void read() override
        {
            Packet::read();
            *this >> location >> yaw >> speed >> unmove<uint16>(state);
        }
        void write() override
        {
            *this << location << yaw << speed << unmove<uint16>(state);
            finish();
        }
    public:
        Vec3 location;
		float yaw;
		float speed;
		EMoveState state;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, Status& status) {
        pk >> status.location >> status.yaw >> status.speed >> unmove<uint16>(status.state);
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const Status& status) {
        pk << status.location << status.yaw << status.speed << unmove<uint16>(status.state);
        return pk;
    }

	class PlayerInfo
            : public sv::Packet {
    public:
        PlayerInfo() : sv::Packet(static_cast<unsigned short>(PacketId::None)) {
        }
        ~PlayerInfo() {
    
        }
    protected:
        void read() override
        {
            Packet::read();
            *this >> objectId >> status;
        }
        void write() override
        {
            *this << objectId << status;
            finish();
        }
    public:
        uint64 objectId;
		Status status;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, PlayerInfo& playerInfo) {
        pk >> playerInfo.objectId >> playerInfo.status;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const PlayerInfo& playerInfo) {
        pk << playerInfo.objectId << playerInfo.status;
        return pk;
    }

}
#pragma warning(pop)
