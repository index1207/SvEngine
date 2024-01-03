#pragma once
#include <core/Packet.hpp>
#include <generated/Packet.hpp>

namespace sv { class Session; }

namespace gen
{
    class PacketHandler
	{
	public:
		static void onReceivePacket(sv::Session* session, PacketId id, std::shared_ptr<sv::Packet> packet)
        {
	        switch (id)
	        {
			case PacketId::CHAT:
				ChatPacketHandler(session, std::static_pointer_cast<Chat>(packet));
				break;
	        }
        }
	private:
		static void ChatPacketHandler(sv::Session* session, std::shared_ptr<Chat> packet);
	};
}
