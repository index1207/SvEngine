#pragma once
#include "Packet.gen.hpp"
#include <core/Packet.hpp>
#include "generated/Enum.gen.hpp"
#include "generated/Protocol.gen.hpp"
#include "generated/Struct.gen.hpp"

using namespace sv;

template<typename T>
using TSharedPtr = std::shared_ptr<T>;

namespace sv { class Session; }

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
			case PacketId::LOGIN_REQ:
			{
				auto packet = Packet::parseFrom<LoginReq>(buffer);
				packet->setHandler(std::bind(LoginReqPacketHandler, std::placeholders::_1, packet));
				return packet;
			}
			case PacketId::ENTER_GAME_REQ:
			{
				auto packet = Packet::parseFrom<EnterGameReq>(buffer);
				packet->setHandler(std::bind(EnterGameReqPacketHandler, std::placeholders::_1, packet));
				return packet;
			}
			case PacketId::LEAVE_GAME_REQ:
			{
				auto packet = Packet::parseFrom<LeaveGameReq>(buffer);
				packet->setHandler(std::bind(LeaveGameReqPacketHandler, std::placeholders::_1, packet));
				return packet;
			}
			case PacketId::MOVE_REQ:
			{
				auto packet = Packet::parseFrom<MoveReq>(buffer);
				packet->setHandler(std::bind(MoveReqPacketHandler, std::placeholders::_1, packet));
				return packet;
			}
            default:
                break;
	        }
            return nullptr;             
        }
	private:
		static void LoginReqPacketHandler(TSharedPtr<Session> session, TSharedPtr<LoginReq> packet);
		static void EnterGameReqPacketHandler(TSharedPtr<Session> session, TSharedPtr<EnterGameReq> packet);
		static void LeaveGameReqPacketHandler(TSharedPtr<Session> session, TSharedPtr<LeaveGameReq> packet);
		static void MoveReqPacketHandler(TSharedPtr<Session> session, TSharedPtr<MoveReq> packet);
	};
}
