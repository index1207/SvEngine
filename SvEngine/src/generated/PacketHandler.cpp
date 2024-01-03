#include <generated/PacketHandler.hpp>
#include <core/Session.hpp>
#include <util/Console.hpp>

using namespace gen;
using namespace sv;

void gen::PacketHandler::ChatPacketHandler(sv::Session* session, std::shared_ptr<Chat> packet)
{
	Console::Log("CHAT PACKET RECV");
}