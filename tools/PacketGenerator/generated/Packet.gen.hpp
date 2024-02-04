#pragma once

namespace gen {
    enum class PacketId {
        None = 0,
		LOGIN_REQ = 1,
		ENTER_GAME_REQ = 2,
		LOGIN_RES = 3,
		ENTER_GAME_RES = 4,
		SPAWN_NOTIFY = 5,
		DESPAWN_NOTIFY = 6    
	};

}