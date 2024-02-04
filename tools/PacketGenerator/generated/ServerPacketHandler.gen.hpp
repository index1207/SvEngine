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
		static void onReceivePacket(TSharedPtr<Session> session, PacketId id, std::span<char> buffer)
        {
	        switch (id)
	        {
            case PacketId::None:
                break;
			case PacketId::LOGIN_REQ:
				LoginReqPacketHandler(session, Packet::parseFrom<LoginReq>(buffer));
				break;
			case PacketId::ENTER_GAME_REQ:
				EnterGameReqPacketHandler(session, Packet::parseFrom<EnterGameReq>(buffer));
				break;
            default:
                break;
	        }
        }
	private:
		static void LoginReqPacketHandler(TSharedPtr<Session> session, TSharedPtr<LoginReq> packet);
		static void EnterGameReqPacketHandler(TSharedPtr<Session> session, TSharedPtr<EnterGameReq> packet);
	};
}
