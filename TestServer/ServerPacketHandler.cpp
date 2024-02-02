#include "generated/ServerPacketHandler.gen.hpp"
#include "core/Session.hpp"

#include "util/Console.hpp"

#include <format>

using namespace gen;

void PacketHandler::EnterGameReqPacketHandler(Session* session, EnterGameReq* packet)
{
	Console::Log(std::format("Recv Packet, id = {}", packet->playerId));
}