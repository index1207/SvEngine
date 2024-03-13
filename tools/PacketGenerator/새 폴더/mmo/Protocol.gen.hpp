#pragma once
#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 4100)
#include "Packet.gen.hpp"

#ifdef __UNREAL__
#include "Network/Packet.h"
#elif __SERVER__
#include "Network/Packet.hpp"
#include "Util/Types.hpp"
#endif

#include <vector>

/* Additional headers. */
#include "Struct.gen.hpp"
namespace gen {
namespace mmo {
    class EnterGameReq
            : public Packet {
    public:
        EnterGameReq() : Packet(static_cast<unsigned short>(PacketId::ENTER_GAME_REQ)) {
        }
        ~EnterGameReq() {
    
        }
    protected:
        virtual void Read() override
        {
            Packet::Read();
            
        }
        virtual void Write() override
        {
            
            Finish();
        }
    public:
        
    };
    
    inline Packet& operator>>(Packet& pk, EnterGameReq& enterGameReq) {
        
        return pk;
    }

    inline Packet& operator<<(Packet& pk, const EnterGameReq& enterGameReq) {
        
        return pk;
    }

	class RoomEventReq
            : public Packet {
    public:
        RoomEventReq() : Packet(static_cast<unsigned short>(PacketId::ROOM_EVENT_REQ)) {
        }
        ~RoomEventReq() {
    
        }
    protected:
        virtual void Read() override
        {
            Packet::Read();
            *this >> room >> reinterpret_cast<uint16&>(event);
        }
        virtual void Write() override
        {
            *this << room << (event);
            Finish();
        }
    public:
        Room room;
		ERoomEvent event;
	
    };
    
    inline Packet& operator>>(Packet& pk, RoomEventReq& roomEventReq) {
        pk >> roomEventReq.room >> reinterpret_cast<uint16&>(roomEventReq.event);
        return pk;
    }

    inline Packet& operator<<(Packet& pk, const RoomEventReq& roomEventReq) {
        pk << roomEventReq.room << (roomEventReq.event);
        return pk;
    }

	class NotifyRoomList
            : public Packet {
    public:
        NotifyRoomList() : Packet(static_cast<unsigned short>(PacketId::NOTIFY_ROOM_LIST)) {
        }
        ~NotifyRoomList() {
    
        }
    protected:
        virtual void Read() override
        {
            Packet::Read();
            *this >> roomList;
        }
        virtual void Write() override
        {
            *this << roomList;
            Finish();
        }
    public:
        std::vector<Room> roomList;
	
    };
    
    inline Packet& operator>>(Packet& pk, NotifyRoomList& notifyRoomList) {
        pk >> notifyRoomList.roomList;
        return pk;
    }

    inline Packet& operator<<(Packet& pk, const NotifyRoomList& notifyRoomList) {
        pk << notifyRoomList.roomList;
        return pk;
    }

	class RoomEventRes
            : public Packet {
    public:
        RoomEventRes() : Packet(static_cast<unsigned short>(PacketId::ROOM_EVENT_RES)) {
        }
        ~RoomEventRes() {
    
        }
    protected:
        virtual void Read() override
        {
            Packet::Read();
            *this >> reinterpret_cast<uint16&>(event) >> success;
        }
        virtual void Write() override
        {
            *this << (event) << success;
            Finish();
        }
    public:
        ERoomEvent event;
		bool success;
	
    };
    
    inline Packet& operator>>(Packet& pk, RoomEventRes& roomEventRes) {
        pk >> reinterpret_cast<uint16&>(roomEventRes.event) >> roomEventRes.success;
        return pk;
    }

    inline Packet& operator<<(Packet& pk, const RoomEventRes& roomEventRes) {
        pk << (roomEventRes.event) << roomEventRes.success;
        return pk;
    }

	class NotifyPlayerList
            : public Packet {
    public:
        NotifyPlayerList() : Packet(static_cast<unsigned short>(PacketId::NOTIFY_PLAYER_LIST)) {
        }
        ~NotifyPlayerList() {
    
        }
    protected:
        virtual void Read() override
        {
            Packet::Read();
            *this >> playerList;
        }
        virtual void Write() override
        {
            *this << playerList;
            Finish();
        }
    public:
        std::vector<PlayerInfo> playerList;
	
    };
    
    inline Packet& operator>>(Packet& pk, NotifyPlayerList& notifyPlayerList) {
        pk >> notifyPlayerList.playerList;
        return pk;
    }

    inline Packet& operator<<(Packet& pk, const NotifyPlayerList& notifyPlayerList) {
        pk << notifyPlayerList.playerList;
        return pk;
    }

}
}
#pragma warning(pop)
