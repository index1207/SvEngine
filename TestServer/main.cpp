#include <iostream>

#include <core/Server.hpp>

using namespace std;
using namespace sv;

int main() {
	Server server = Server::open();
	server.run(Endpoint(IpAddress::Any, 9999));

	while (true) {
		server.loop();
	}
}