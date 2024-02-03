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

            default:
                break;                         
	        }
        }
	private:

	};
}
