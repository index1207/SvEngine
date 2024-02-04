#pragma once
#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 4100)
#include "Packet.gen.hpp"

#include <core/Packet.hpp>
#include "util/Types.hpp"

#include <vector>

/* Additional generated packet headers. */


namespace gen {
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
            *this >> objectId >> x >> y >> z >> yaw;
        }
        void write() override
        {
            *this << objectId << x << y << z << yaw;
            finish();
        }
    public:
        uint64 objectId;
		float x;
		float y;
		float z;
		float yaw;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, PlayerInfo& playerInfo) {
        pk >> playerInfo.objectId >> playerInfo.x >> playerInfo.y >> playerInfo.z >> playerInfo.yaw;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const PlayerInfo& playerInfo) {
        pk << playerInfo.objectId << playerInfo.x << playerInfo.y << playerInfo.z << playerInfo.yaw;
        return pk;
    }

}
#pragma warning(pop)
