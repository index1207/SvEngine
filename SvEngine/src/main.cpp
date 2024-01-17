#include <iostream>

#include <net/IoSystem.hpp>
#include <net/Dns.hpp>

#include "core/Server.hpp"
#include "core/Session.hpp"
#include "util/Console.hpp"

#include <generated/Example.gen.hpp>

using namespace net;
using namespace std;

using namespace sv;

class TestClient : public Session
{
public:
    TestClient() = default;
    ~TestClient() override = default;
public:
    void onConnected() override
    {
        Session::onConnected();
        Console::Log("Connected");
        
        send(make_unique<gen::LoginRes>().get());
    }
    void onDisconnected() override
    {
        Session::onDisconnected();
        Console::Log("Disconnected");
    }
    void onReceive(std::span<char> buffer, int length) override
    {
        Session::onReceivePacket(buffer, length);
    }
private:
};
    
int main() {
    auto server = Server::open<TestClient>();
    auto endpoint = Endpoint(IpAddress::Any, 9999);
    try {
        server.run(endpoint);
        Console::Log(("Server is running on " + endpoint.toString()).c_str());

        system("pause > nul");
    }
    catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }
}