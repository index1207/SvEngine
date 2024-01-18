#pragma once

namespace gen {
    enum class PacketId {
		ENTER_GAME_REQ = 1,
		ENTER_GAME_RES = 2,
		LOGIN_REQ = 3,
		LOGIN_RES = 4,
		MESSAGE = 5,
		SEND_MESSAGE_REQ = 6    
	};

}