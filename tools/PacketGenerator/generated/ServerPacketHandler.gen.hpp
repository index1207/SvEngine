#pragma once
#include "generated/Packet.gen.hpp"
#include <core/Packet.hpp>
#include "generated/Example.gen.hpp"

using namespace sv;                                             

namespace sv { class Session; }

namespace gen
{
    class PacketHandler
	{
	public:
		static void onReceivePacket(Session* session, PacketId id, Packet* packet)
        {
	        switch (id)
	        {
			case PacketId::ENTER_GAME_REQ:
				EnterGameReqPacketHandler(session, static_cast<EnterGameReq*>(packet));
				break;
	        }
        }
	private:
		static void EnterGameReqPacketHandler(Session* session, EnterGameReq* packet);
	};
}
