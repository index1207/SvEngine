#pragma once
#include <core/Packet.hpp>
#include <util/Console.hpp>

#include <generated/Packet.hpp>

namespace sv { class Session; }

namespace gen
{
    class PacketHandler
	{
	public:
		static void onReceivePacket(sv::Session* session, PacketId id, sv::Packet* packet)
        {
	        switch (id)
	        {
			case PacketId::A:
				APacketHandler(session, static_cast<A*>(packet));
				break;
			case PacketId::B:
				BPacketHandler(session, static_cast<B*>(packet));
				break;
			case PacketId::C:
				CPacketHandler(session, static_cast<C*>(packet));
				break;
			case PacketId::D:
				DPacketHandler(session, static_cast<D*>(packet));
				break;
	        }
        }
	private:
		static void APacketHandler(sv::Session* session, A* packet);
		static void BPacketHandler(sv::Session* session, B* packet);
		static void CPacketHandler(sv::Session* session, C* packet);
		static void DPacketHandler(sv::Session* session, D* packet);
	};
}
