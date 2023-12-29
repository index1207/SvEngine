#pragma once
#include <core/Packet.hpp>

#include "DefinedPacket.hpp"

namespace gen
{
    class PacketHandler
	{
	public:
		static void onReceivePacket(PacketId id, sv::Packet* packet)
        {
	        switch (id)
	        {
	        case PacketId::TEST:
				auto test = static_cast<Test*>(packet);
				TestPacketHandler(test);
				break;
	        }
        }
	private:
		static void TestPacketHandler(Test* packet);
	};
}
