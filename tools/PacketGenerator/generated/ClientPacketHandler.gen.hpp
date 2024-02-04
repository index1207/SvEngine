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
		static void onReceivePacket(TSharedPtr<Session> session, PacketId id, std::span<char> buffer)
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
			case PacketId::SPAWN_NOTIFY:
				SpawnNotifyPacketHandler(session, Packet::parseFrom<SpawnNotify>(buffer));
				break;
			case PacketId::DESPAWN_NOTIFY:
				DespawnNotifyPacketHandler(session, Packet::parseFrom<DespawnNotify>(buffer));
				break;
            default:
                break;                         
	        }
        }
	private:
		static void LoginResPacketHandler(TSharedPtr<Session> session, TSharedPtr<LoginRes> packet);
		static void EnterGameResPacketHandler(TSharedPtr<Session> session, TSharedPtr<EnterGameRes> packet);
		static void SpawnNotifyPacketHandler(TSharedPtr<Session> session, TSharedPtr<SpawnNotify> packet);
		static void DespawnNotifyPacketHandler(TSharedPtr<Session> session, TSharedPtr<DespawnNotify> packet);
	};
}
