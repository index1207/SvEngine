#include "pch.h"

using namespace std;

int main()
{
	auto ep = Endpoint({IP_ADDRESS}, {PORT});
	try {
		auto server = Server::open<{SESSION_CLASS}>();
		server->run(ep);

		Console::Log("Server is running on " + ep.toString());

		while (true) { Sleep(100); }
	}
	catch (exception& e) {
		Console::Log(e.what());
	}
}