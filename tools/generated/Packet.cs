/*
    TODO: Copy to your project.
*/

namespace Gen
{
    public enum PacketId
    {
		ENTER_GAME_REQ = 1,
		ENTER_GAME_RES = 2,
		LOGIN_REQ = 3,
		LOGIN_RES = 4
    }

    class EnterGameReq : Sv.Packet {

    }

	class EnterGameRes : Sv.Packet {

    }

	class LoginReq : Sv.Packet {

    }

	class LoginRes : Sv.Packet {

    }

}
