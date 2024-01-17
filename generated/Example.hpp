#pragma once
#pragma warning(push)
#pragma warning(disable: 26495)
#include <core/Packet.hpp>
#include <util/Types.hpp>

#include <vector>

#include "./Test.hpp"

namespace gen {
    enum class PacketId {
		ENTER_GAME_REQ = 1,
		ENTER_GAME_RES = 2,
		LOGIN_REQ = 3,
		LOGIN_RES = 4
    };

    class EnterGameReq
            : public sv::Packet {
    public:
        EnterGameReq() : sv::Packet(static_cast<unsigned short>(PacketId::ENTER_GAME_REQ)) {
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
        int32 playerId;
	
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
        std::vector<int32> playerList;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, EnterGameRes& enterGameRes) {
        pk >> enterGameRes.playerList;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const EnterGameRes& enterGameRes) {
        pk << enterGameRes.playerList;
        return pk;
    }

	class LoginReq
            : public sv::Packet {
    public:
        LoginReq() : sv::Packet(static_cast<unsigned short>(PacketId::LOGIN_REQ)) {
        }
        ~LoginReq() {
    
        }
    public:
        void read() override
        {
            Packet::read();
            *this >> usrId >> usrPwd;
        }
        void write() override
        {
            *this << usrId << usrPwd;
            finish();
        }
    public:
        std::string usrId;
		std::string usrPwd;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, LoginReq& loginReq) {
        pk >> loginReq.usrId >> loginReq.usrPwd;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const LoginReq& loginReq) {
        pk << loginReq.usrId << loginReq.usrPwd;
        return pk;
    }

	class LoginRes
            : public sv::Packet {
    public:
        LoginRes() : sv::Packet(static_cast<unsigned short>(PacketId::LOGIN_RES)) {
        }
        ~LoginRes() {
    
        }
    public:
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
    
    inline sv::Packet& operator>>(sv::Packet& pk, LoginRes& loginRes) {
        pk >> loginRes.isSuccess;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const LoginRes& loginRes) {
        pk << loginRes.isSuccess;
        return pk;
    }

}
#pragma warning(pop)
