#pragma once
#include "Packet.gen.hpp"
#include "generated/Enum.gen.hpp"
#include "generated/Protocol.gen.hpp"
#include "generated/Struct.gen.hpp"

using namespace sv;
                         
#ifdef __UNREAL__
#include "winsock2.h"
#include "Network/Packet.h"

using Session = class FSession;

#define STATIC_POINTER_CAST(to, from) StaticCastSharedPtr<to>(from)
#else
template<typename T>
using TSharedPtr = std::shared_ptr<T>;
template<typename T>
using TFunction = std::function<T>;

#define STATIC_POINTER_CAST(to, from) std::static_pointer_cast<to>(from)

namespace sv { class Session; }
#endif

#define HANDLE_PACKET(pckname, buffer) std::bind(pckname##PacketHandler, std::placeholders::_1, STATIC_POINTER_CAST(pckname, Packet::parseFrom<pckname>(buffer)));
#define BIND_HANDLER(pckname, buffer) std::bind(pckname##PacketHandler, std::placeholders::_1, STATIC_POINTER_CAST(pckname, Packet::parseFrom<pckname>(buffer)));

namespace gen
{
    class PacketHandler
	{
    	using Handler = TFunction<bool(TSharedPtr<Session>)>;
	public:
		static Handler getHandler(std::span<char> buffer)
        {
            gen::PacketId id = gen::PacketId::None;
			std::memcpy(&id, buffer.data(), sizeof(unsigned short));
			id = (gen::PacketId)ntohs((u_short)id);
            
            switch (id)
            {
			case LOGIN_RES:
				return BIND_HANDLER(LoginRes, buffer);
			case ENTER_GAME_RES:
				return BIND_HANDLER(EnterGameRes, buffer);
			case LEAVE_GAME_RES:
				return BIND_HANDLER(LeaveGameRes, buffer);
			case SPAWN_NOTIFY:
				return BIND_HANDLER(SpawnNotify, buffer);
			case DESPAWN_NOTIFY:
				return BIND_HANDLER(DespawnNotify, buffer);
			case MOVE_RES:
				return BIND_HANDLER(MoveRes, buffer);
            }
            return nullptr;
        }
        static bool handlePacket(TSharedPtr<Session> session, std::span<char> buffer)
        {
            auto handler = getHandler(buffer);
            if (!handler || !session)
                return false;
            return handler(session);
        }
		static bool LoginResPacketHandler(TSharedPtr<Session> session, TSharedPtr<LoginRes> packet);
		static bool EnterGameResPacketHandler(TSharedPtr<Session> session, TSharedPtr<EnterGameRes> packet);
		static bool LeaveGameResPacketHandler(TSharedPtr<Session> session, TSharedPtr<LeaveGameRes> packet);
		static bool SpawnNotifyPacketHandler(TSharedPtr<Session> session, TSharedPtr<SpawnNotify> packet);
		static bool DespawnNotifyPacketHandler(TSharedPtr<Session> session, TSharedPtr<DespawnNotify> packet);
		static bool MoveResPacketHandler(TSharedPtr<Session> session, TSharedPtr<MoveRes> packet);
	};
}
