#pragma once
#pragma warning(push)
#pragma warning(disable: 26495)
#include "Packet.gen.hpp"

#include <core/Packet.hpp>
#include "util/Types.hpp"

#include <vector>

/* Additional generated packet headers. */
#include "Struct.gen.hpp"

namespace gen {
    class LoginReq
            : public sv::Packet {
    public:
        LoginReq() : sv::Packet(static_cast<unsigned short>(PacketId::LOGIN_REQ)) {
        }
        ~LoginReq() {
    
        }
    protected:
        void read() override
        {
            Packet::read();
            
        }
        void write() override
        {
            
            finish();
        }
    public:
        
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, LoginReq& loginReq) {
        
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const LoginReq& loginReq) {
        
        return pk;
    }

	class LoginRes
            : public sv::Packet {
    public:
        LoginRes() : sv::Packet(static_cast<unsigned short>(PacketId::LOGIN_RES)) {
        }
        ~LoginRes() {
    
        }
    protected:
        void read() override
        {
            Packet::read();
            *this >> isSuccess >> playerList;
        }
        void write() override
        {
            *this << isSuccess << playerList;
            finish();
        }
    public:
        bool isSuccess;
		std::vector<PlayerInfo> playerList;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, LoginRes& loginRes) {
        pk >> loginRes.isSuccess >> loginRes.playerList;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const LoginRes& loginRes) {
        pk << loginRes.isSuccess << loginRes.playerList;
        return pk;
    }

	class EnterGameReq
            : public sv::Packet {
    public:
        EnterGameReq() : sv::Packet(static_cast<unsigned short>(PacketId::ENTER_GAME_REQ)) {
        }
        ~EnterGameReq() {
    
        }
    protected:
        void read() override
        {
            Packet::read();
            *this >> playerIdx;
        }
        void write() override
        {
            *this << playerIdx;
            finish();
        }
    public:
        uint64 playerIdx;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, EnterGameReq& enterGameReq) {
        pk >> enterGameReq.playerIdx;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const EnterGameReq& enterGameReq) {
        pk << enterGameReq.playerIdx;
        return pk;
    }

	class EnterGameRes
            : public sv::Packet {
    public:
        EnterGameRes() : sv::Packet(static_cast<unsigned short>(PacketId::ENTER_GAME_RES)) {
        }
        ~EnterGameRes() {
    
        }
    protected:
        void read() override
        {
            Packet::read();
            *this >> isSuccess;
        }
        void write() override
        {
            *this << isSuccess;
            finish();
        }
    public:
        bool isSuccess;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, EnterGameRes& enterGameRes) {
        pk >> enterGameRes.isSuccess;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const EnterGameRes& enterGameRes) {
        pk << enterGameRes.isSuccess;
        return pk;
    }

}
#pragma warning(pop)
