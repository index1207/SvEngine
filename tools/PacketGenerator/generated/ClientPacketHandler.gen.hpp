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
		static TSharedPtr<Packet> parsePacket(PacketId id, std::span<char> buffer)
        {
	        switch (id)
	        {
            case PacketId::None:
                break;             
			case PacketId::LOGIN_RES:
			{{
				auto packet = Packet::parseFrom<LoginRes>(buffer);
				packet->setHandler(std::bind(LoginResPacketHandler, std::placeholders::_1, packet));
				return packet;
			}}
			case PacketId::ENTER_GAME_RES:
			{{
				auto packet = Packet::parseFrom<EnterGameRes>(buffer);
				packet->setHandler(std::bind(EnterGameResPacketHandler, std::placeholders::_1, packet));
				return packet;
			}}
			case PacketId::LEAVE_GAME_RES:
			{{
				auto packet = Packet::parseFrom<LeaveGameRes>(buffer);
				packet->setHandler(std::bind(LeaveGameResPacketHandler, std::placeholders::_1, packet));
				return packet;
			}}
			case PacketId::SPAWN_NOTIFY:
			{{
				auto packet = Packet::parseFrom<SpawnNotify>(buffer);
				packet->setHandler(std::bind(SpawnNotifyPacketHandler, std::placeholders::_1, packet));
				return packet;
			}}
			case PacketId::DESPAWN_NOTIFY:
			{{
				auto packet = Packet::parseFrom<DespawnNotify>(buffer);
				packet->setHandler(std::bind(DespawnNotifyPacketHandler, std::placeholders::_1, packet));
				return packet;
			}}
            default:
                break;                         
	        }
            return nullptr;             
        }
	private:
		static void LoginResPacketHandler(TSharedPtr<Session> session, TSharedPtr<LoginRes> packet);
		static void EnterGameResPacketHandler(TSharedPtr<Session> session, TSharedPtr<EnterGameRes> packet);
		static void LeaveGameResPacketHandler(TSharedPtr<Session> session, TSharedPtr<LeaveGameRes> packet);
		static void SpawnNotifyPacketHandler(TSharedPtr<Session> session, TSharedPtr<SpawnNotify> packet);
		static void DespawnNotifyPacketHandler(TSharedPtr<Session> session, TSharedPtr<DespawnNotify> packet);
	};
}
