#include "generated/ServerPacketHandler.gen.hpp"
#include "core/Session.hpp"

#include "util/Console.hpp"

#include <format>

using namespace gen;

void PacketHandler::EnterGameReqPacketHandler(Session* session, TSharedPtr<EnterGameReq> packet)
{
	Console::Log(std::format("Recv Packet, id = {}", packet->playerId));

	auto res = new EnterGameRes;
	res->playerList = std::vector<Int32>({1,2,3});
	session->send(res);
}