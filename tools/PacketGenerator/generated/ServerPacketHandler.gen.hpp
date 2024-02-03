#pragma once
#include "Packet.gen.hpp"
#include <core/Packet.hpp>
#include "generated/Example.gen.hpp"

using namespace sv;
                         
template<typename T>                         
using TSharedPtr = std::shared_ptr<T>;
                         
namespace sv { class Session; }

namespace gen
{
    class PacketHandler
	{
	public:
		static void onReceivePacket(Session* session, PacketId id, std::span<char> buffer)
        {
	        switch (id)
	        {
			case PacketId::ENTER_GAME_REQ:
				EnterGameReqPacketHandler(session, Packet::parseFrom<EnterGameReq>(buffer));
				break;
	        }
        }
	private:
		static void EnterGameReqPacketHandler(Session* session, TSharedPtr<EnterGameReq> packet);
	};
}
