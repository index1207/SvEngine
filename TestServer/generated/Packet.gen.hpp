#pragma once

namespace gen {
    enum class PacketId {
        None = 0,
		LOGIN_REQ = 1,
		ENTER_GAME_REQ = 2,
		LEAVE_GAME_REQ = 3,
		LOGIN_RES = 4,
		ENTER_GAME_RES = 5,
		LEAVE_GAME_RES = 6,
		SPAWN_NOTIFY = 7,
		DESPAWN_NOTIFY = 8    
	};

}