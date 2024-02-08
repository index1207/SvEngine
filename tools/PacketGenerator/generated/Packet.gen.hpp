#pragma once

template<class T> inline T& unmove(T&& t) { return static_cast<T&>(t); }

namespace gen {
    enum PacketId : uint16 {
        None = 0,
		LOGIN_REQ = 1,
		ENTER_GAME_REQ = 2,
		LEAVE_GAME_REQ = 3,
		MOVE_REQ = 4,
		LOGIN_RES = 5,
		ENTER_GAME_RES = 6,
		LEAVE_GAME_RES = 7,
		SPAWN_NOTIFY = 8,
		DESPAWN_NOTIFY = 9,
		MOVE_RES = 10    
	};

}