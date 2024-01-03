#include <iostream>
#include <net/IoSystem.hpp>
#include <net/Dns.hpp>

#include "core/Server.hpp"
#include "core/Session.hpp"
#include "util/Console.hpp"

using namespace net;
using namespace sv;
using namespace std;

class TestClient : public Session
{
public:
    TestClient() = default;
    ~TestClient() override = default;
public:
    void onConnected() override
    {
        Console::Log("Connected");
    }
    void onDisconnected() override
    {
        Console::Log("Disconnected");
    }
    void onReceive(std::span<char> buffer, int length) override
    {
        Session::onReceive(buffer, length);
    }
    void onSend(int length) override
    {
        Session::onSend(length);
    }
private:
};

int main() {
    auto server = Server::open<TestClient>();
    auto endpoint = Endpoint(IpAddress::Any, 9999);
    try {
        server.run(endpoint);
        Console::Log(("Server is running on " + endpoint.toString()).c_str());

        getchar();
    }
    catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }
}