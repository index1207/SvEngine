#pragma once
#include <generated/Packet.gen.hpp>
#include <core/Packet.hpp>
#include <generated/Example.gen.hpp>

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
	        }
        }
	private:
		static void EnterGameReqPacketHandler(sv::Session* session, EnterGameReq* packet);
	};
}
