#include <iostream>

#include <core/Server.hpp>
#include <util/Console.hpp>

#include <net/Socket.hpp>
#include <net/Exception.hpp>

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

		std::string data = "Hello, World!";
		send(data);
	}
	
	virtual void onDisconnected() override
	{
		Console::Log("Disconnected");
	}

	virtual void onReceive(std::span<char> buffer, int length) override
	{
		Console::Log("RECV");
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