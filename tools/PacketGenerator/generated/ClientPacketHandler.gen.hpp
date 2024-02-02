#pragma once
#include "generated/Packet.gen.hpp"
#include "Network/Packet.h"
#include "generated/Example.gen.hpp"

using namespace sv;                                                  
using Session = class FSession;
                         
namespace gen
{
    class PacketHandler
	{
	public:
		static void onReceivePacket(Session* session, PacketId id, Packet* packet)
        {
	        switch (id)
	        {
			case PacketId::ENTER_GAME_RES:
				EnterGameResPacketHandler(session, static_cast<EnterGameRes*>(packet));
				break;
	        }
        }
	private:
		static void EnterGameResPacketHandler(Session* session, EnterGameRes* packet);
	};
}
