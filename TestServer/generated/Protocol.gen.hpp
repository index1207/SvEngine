#pragma once
#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 4100)
#include "Packet.gen.hpp"

#include <vector>

/* Additional headers. */
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
            *this >> isSuccess >> player;
        }
        void write() override
        {
            *this << isSuccess << player;
            finish();
        }
    public:
        bool isSuccess;
		PlayerInfo player;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, EnterGameRes& enterGameRes) {
        pk >> enterGameRes.isSuccess >> enterGameRes.player;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const EnterGameRes& enterGameRes) {
        pk << enterGameRes.isSuccess << enterGameRes.player;
        return pk;
    }

	class LeaveGameReq
            : public sv::Packet {
    public:
        LeaveGameReq() : sv::Packet(static_cast<unsigned short>(PacketId::LEAVE_GAME_REQ)) {
        }
        ~LeaveGameReq() {
    
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
    
    inline sv::Packet& operator>>(sv::Packet& pk, LeaveGameReq& leaveGameReq) {
        
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const LeaveGameReq& leaveGameReq) {
        
        return pk;
    }

	class LeaveGameRes
            : public sv::Packet {
    public:
        LeaveGameRes() : sv::Packet(static_cast<unsigned short>(PacketId::LEAVE_GAME_RES)) {
        }
        ~LeaveGameRes() {
    
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
    
    inline sv::Packet& operator>>(sv::Packet& pk, LeaveGameRes& leaveGameRes) {
        
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const LeaveGameRes& leaveGameRes) {
        
        return pk;
    }

	class SpawnNotify
            : public sv::Packet {
    public:
        SpawnNotify() : sv::Packet(static_cast<unsigned short>(PacketId::SPAWN_NOTIFY)) {
        }
        ~SpawnNotify() {
    
        }
    protected:
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
        std::vector<PlayerInfo> playerList;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, SpawnNotify& spawnNotify) {
        pk >> spawnNotify.playerList;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const SpawnNotify& spawnNotify) {
        pk << spawnNotify.playerList;
        return pk;
    }

	class DespawnNotify
            : public sv::Packet {
    public:
        DespawnNotify() : sv::Packet(static_cast<unsigned short>(PacketId::DESPAWN_NOTIFY)) {
        }
        ~DespawnNotify() {
    
        }
    protected:
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
        std::vector<uint64> playerList;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, DespawnNotify& despawnNotify) {
        pk >> despawnNotify.playerList;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const DespawnNotify& despawnNotify) {
        pk << despawnNotify.playerList;
        return pk;
    }

}
#pragma warning(pop)
