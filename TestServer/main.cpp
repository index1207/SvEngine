#include <iostream>

#include <core/Server.hpp>
#include <util/Console.hpp>

#include <net/Socket.hpp>
#include <net/Exception.hpp>

#include "generated/ServerPacketHandler.gen.hpp"

using namespace std;
using namespace net;
using namespace sv;

class GameSession : public Session
{
public:
	GameSession() {}
	~GameSession() {}
public:
	virtual void onConnected() override
	{
		Console::Log("Connected");
	}
	
	virtual void onDisconnected() override
	{
		Console::Log("Disconnected");
	}

	virtual void onReceive(std::span<char> buffer, int length) override
	{
		if (length <= 2)
			return;

		gen::PacketId id = (gen::PacketId)0;
		std::memcpy(&id, buffer.data(), sizeof(unsigned short));
		id = static_cast<gen::PacketId>(htons(static_cast<unsigned short>(id)));

		gen::PacketHandler::onReceivePacket(this, id, buffer);
	}
};

int main()
{
	try {
		auto server = Server::open<GameSession>();
		server->run(Endpoint(IpAddress::Any, 9999));

		system("pause > nul");
	}
	catch (std::exception& e) {
		Console::Log(e.what());
	}
}