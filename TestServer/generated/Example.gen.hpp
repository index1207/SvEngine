#pragma once
#pragma warning(push)
#pragma warning(disable: 26495)
#include "generated/Packet.gen.hpp"

#include <core/Packet.hpp>
#include "util/Types.hpp"

#include <vector>

/* Additional generated packet headers. */


namespace gen {
    class EnterGameReq
            : public sv::Packet {
    public:
        EnterGameReq() : sv::Packet(static_cast<unsigned short>(PacketId::ENTER_GAME_REQ)) {
            playerId = 0;
        }
        ~EnterGameReq() {
    
        }
    public:
        void read() override
        {
            Packet::read();
            *this >> playerId;
        }
        void write() override
        {
            *this << playerId;
            finish();
        }
    public:
        Int32 playerId;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, EnterGameReq& enterGameReq) {
        pk >> enterGameReq.playerId;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const EnterGameReq& enterGameReq) {
        pk << enterGameReq.playerId;
        return pk;
    }

	class EnterGameRes
            : public sv::Packet {
    public:
        EnterGameRes() : sv::Packet(static_cast<unsigned short>(PacketId::ENTER_GAME_RES)) {
        }
        ~EnterGameRes() {
    
        }
    public:
        void read() override
        {
            Packet::read();
            *this >> playerList;
        }
        void write() override
        {
            *this << playerList;
            finish();
        }
    public:
        std::vector<Int32> playerList;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, EnterGameRes& enterGameRes) {
        pk >> enterGameRes.playerList;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const EnterGameRes& enterGameRes) {
        pk << enterGameRes.playerList;
        return pk;
    }

}
#pragma warning(pop)
