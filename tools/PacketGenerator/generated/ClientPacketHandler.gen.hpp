#pragma once
#include "Packet.gen.hpp"
#include "Network/Packet.h"
#include "generated/Example.gen.hpp"

using namespace sv;                                                  
using Session = class FSession;                         
                         
namespace gen
{
    class PacketHandler
	{
	public:
		static void onReceivePacket(Session* session, PacketId id, std::span<char> buffer)
        {
	        switch (id)
	        {
			case PacketId::ENTER_GAME_RES:
				EnterGameResPacketHandler(session, Packet::parseFrom<EnterGameRes>(buffer));
				break;
	        }
        }
	private:
		static void EnterGameResPacketHandler(Session* session, TSharedPtr<EnterGameRes> packet);
	};
}
