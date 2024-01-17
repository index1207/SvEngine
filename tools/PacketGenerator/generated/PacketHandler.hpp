#pragma once
#include <core/Packet.hpp>
#include <generated/Example.hpp>

namespace sv { class Session; }

namespace gen
{
    class PacketHandler
	{
	public:
		static void onReceivePacket(sv::Session* session, PacketId id, sv::Packet* packet)
        {
	        switch (id)
	        {
			case PacketId::ENTER_GAME_REQ:
				EnterGameReqPacketHandler(session, static_cast<EnterGameReq*>(packet));
				break;
			case PacketId::ENTER_GAME_RES:
				EnterGameResPacketHandler(session, static_cast<EnterGameRes*>(packet));
				break;
			case PacketId::LOGIN_REQ:
				LoginReqPacketHandler(session, static_cast<LoginReq*>(packet));
				break;
			case PacketId::LOGIN_RES:
				LoginResPacketHandler(session, static_cast<LoginRes*>(packet));
				break;
	        }
        }
	private:
		static void EnterGameReqPacketHandler(sv::Session* session, EnterGameReq* packet);
		static void EnterGameResPacketHandler(sv::Session* session, EnterGameRes* packet);
		static void LoginReqPacketHandler(sv::Session* session, LoginReq* packet);
		static void LoginResPacketHandler(sv::Session* session, LoginRes* packet);
	};
}
