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
#include "Struct.gen.hpp"

namespace gen {
    class LoginReq
            : public sv::Packet {
    public:
        LoginReq() : Packet(static_cast<unsigned short>(PacketId::LOGIN_REQ)) {
        }
        ~LoginReq() {
    
        }
    protected:
        virtual void read() override
        {
            Packet::read();
            
        }
        virtual void write() override
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
        LoginRes() : Packet(static_cast<unsigned short>(PacketId::LOGIN_RES)) {
        }
        ~LoginRes() {
    
        }
    protected:
        virtual void read() override
        {
            Packet::read();
            *this >> isSuccess >> playerList;
        }
        virtual void write() override
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
        EnterGameReq() : Packet(static_cast<unsigned short>(PacketId::ENTER_GAME_REQ)) {
        }
        ~EnterGameReq() {
    
        }
    protected:
        virtual void read() override
        {
            Packet::read();
            *this >> playerIdx;
        }
        virtual void write() override
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
        EnterGameRes() : Packet(static_cast<unsigned short>(PacketId::ENTER_GAME_RES)) {
        }
        ~EnterGameRes() {
    
        }
    protected:
        virtual void read() override
        {
            Packet::read();
            *this >> isSuccess >> player;
        }
        virtual void write() override
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
        LeaveGameReq() : Packet(static_cast<unsigned short>(PacketId::LEAVE_GAME_REQ)) {
        }
        ~LeaveGameReq() {
    
        }
    protected:
        virtual void read() override
        {
            Packet::read();
            
        }
        virtual void write() override
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
        LeaveGameRes() : Packet(static_cast<unsigned short>(PacketId::LEAVE_GAME_RES)) {
        }
        ~LeaveGameRes() {
    
        }
    protected:
        virtual void read() override
        {
            Packet::read();
            
        }
        virtual void write() override
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
        SpawnNotify() : Packet(static_cast<unsigned short>(PacketId::SPAWN_NOTIFY)) {
        }
        ~SpawnNotify() {
    
        }
    protected:
        virtual void read() override
        {
            Packet::read();
            *this >> playerList;
        }
        virtual void write() override
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
        DespawnNotify() : Packet(static_cast<unsigned short>(PacketId::DESPAWN_NOTIFY)) {
        }
        ~DespawnNotify() {
    
        }
    protected:
        virtual void read() override
        {
            Packet::read();
            *this >> playerList;
        }
        virtual void write() override
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

	class MoveReq
            : public sv::Packet {
    public:
        MoveReq() : Packet(static_cast<unsigned short>(PacketId::MOVE_REQ)) {
        }
        ~MoveReq() {
    
        }
    protected:
        virtual void read() override
        {
            Packet::read();
            *this >> status;
        }
        virtual void write() override
        {
            *this << status;
            finish();
        }
    public:
        Status status;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, MoveReq& moveReq) {
        pk >> moveReq.status;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const MoveReq& moveReq) {
        pk << moveReq.status;
        return pk;
    }

	class MoveRes
            : public sv::Packet {
    public:
        MoveRes() : Packet(static_cast<unsigned short>(PacketId::MOVE_RES)) {
        }
        ~MoveRes() {
    
        }
    protected:
        virtual void read() override
        {
            Packet::read();
            *this >> info;
        }
        virtual void write() override
        {
            *this << info;
            finish();
        }
    public:
        PlayerInfo info;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, MoveRes& moveRes) {
        pk >> moveRes.info;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const MoveRes& moveRes) {
        pk << moveRes.info;
        return pk;
    }

}
#pragma warning(pop)
