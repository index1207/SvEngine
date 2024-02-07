#pragma once
#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 4100)
#include "Packet.gen.hpp"

#include <vector>

/* Additional headers. */


namespace gen {
    class vec3
            : public sv::Packet {
    public:
        vec3() : sv::Packet(static_cast<unsigned short>(PacketId::None)) {
        }
        ~vec3() {
    
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
    
    inline sv::Packet& operator>>(sv::Packet& pk, vec3& vec3) {
        pk >> vec3.x >> vec3.y >> vec3.z;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const vec3& vec3) {
        pk << vec3.x << vec3.y << vec3.z;
        return pk;
    }

	class vec4
            : public sv::Packet {
    public:
        vec4() : sv::Packet(static_cast<unsigned short>(PacketId::None)) {
        }
        ~vec4() {
    
        }
    protected:
        void read() override
        {
            Packet::read();
            *this >> x >> y >> z >> yaw;
        }
        void write() override
        {
            *this << x << y << z << yaw;
            finish();
        }
    public:
        float x;
		float y;
		float z;
		float yaw;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, vec4& vec4) {
        pk >> vec4.x >> vec4.y >> vec4.z >> vec4.yaw;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const vec4& vec4) {
        pk << vec4.x << vec4.y << vec4.z << vec4.yaw;
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
            *this >> objectId >> location;
        }
        void write() override
        {
            *this << objectId << location;
            finish();
        }
    public:
        uint64 objectId;
		vec4 location;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, PlayerInfo& playerInfo) {
        pk >> playerInfo.objectId >> playerInfo.location;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const PlayerInfo& playerInfo) {
        pk << playerInfo.objectId << playerInfo.location;
        return pk;
    }

}
#pragma warning(pop)
