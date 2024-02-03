#pragma once
#include "Packet.gen.hpp"
#include "Network/Packet.h"
#include "generated/Enum.gen.hpp"
#include "generated/Protocol.gen.hpp"
#include "generated/Struct.gen.hpp"

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
            case PacketId::None:
                break;             
			case PacketId::LOGIN_RES:
				LoginResPacketHandler(session, Packet::parseFrom<LoginRes>(buffer));
				break;
			case PacketId::ENTER_GAME_RES:
				EnterGameResPacketHandler(session, Packet::parseFrom<EnterGameRes>(buffer));
				break;
            default:
                break;                         
	        }
        }
	private:
		static void LoginResPacketHandler(Session* session, TSharedPtr<LoginRes> packet);
		static void EnterGameResPacketHandler(Session* session, TSharedPtr<EnterGameRes> packet);
	};
}
