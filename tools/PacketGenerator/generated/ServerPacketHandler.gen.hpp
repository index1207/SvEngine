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
