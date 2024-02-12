#include "pch.h"
#include "GameSession.hpp"

using namespace std;

int main()
{
	auto ep = Endpoint(IpAddress::Any, 9999);
	try {
		auto server = Server::open<GameSession>();
		server->run(ep);

		Console::Log("Server is running on " + ep.toString());

		while (true) { Sleep(100); }
	}
	catch (exception& e) {
		Console::Log(e.what());
	}
}