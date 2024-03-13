#pragma once

template<class T> inline T& unmove(T&& t) { return static_cast<T&>(t); }

namespace gen {
namespace mmo {
    enum PacketId : uint16 {
        NONE = 0,
		ENTER_GAME_REQ = 1,
		ROOM_EVENT_REQ = 2,
		NOTIFY_ROOM_LIST = 3,
		ROOM_EVENT_RES = 4,
		NOTIFY_PLAYER_LIST = 5    
	};

}
}