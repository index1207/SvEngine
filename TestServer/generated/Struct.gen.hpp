#pragma once
#pragma warning(push)
#pragma warning(disable: 26495)
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
            *this >> objectId;
        }
        void write() override
        {
            *this << objectId;
            finish();
        }
    public:
        uint64 objectId;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, PlayerInfo& playerInfo) {
        pk >> playerInfo.objectId;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const PlayerInfo& playerInfo) {
        pk << playerInfo.objectId;
        return pk;
    }

}
#pragma warning(pop)
