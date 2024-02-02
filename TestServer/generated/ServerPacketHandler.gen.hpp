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
		static void onReceivePacket(Session* session, PacketId id, std::span<char> buffer)
        {
			Packet* pk;
	        switch (id)
	        {
			case PacketId::ENTER_GAME_REQ:
				pk = new EnterGameReq;
				pk->parse(buffer);
				EnterGameReqPacketHandler(session, static_cast<EnterGameReq*>(pk));
				break;
	        }
        }
	private:
		static void EnterGameReqPacketHandler(Session* session, EnterGameReq* packet);
	};
}
