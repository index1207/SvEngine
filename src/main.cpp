#include <iostream>
#include <string>
#include <mutex>

#include "core/Server.hpp"
#include "net/Dns.hpp"
#include "packet.hpp"

using namespace net;
using namespace sv;
using namespace gen;

class TestClient : public sv::Session
{
public:
    TestClient() = default;
    ~TestClient() override = default;
public:
    void onConnected() override
    {
        std::cout << "Connected" << '\n';
    }
    void onDisconnected() override
    {
        std::cout << "Disconnected" << '\n';
    }
    void onReceive(std::span<char> buffer, int length) override
    {
        std::string str = std::string(buffer.begin(), buffer.end());
        std::cout << "Recv " << str << '\n';
        send(str);
    }
    void onSend(std::span<char> buffer, int length) override
    {
        std::cout << "Sent " << length << '\n';
    }
private:
};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    auto server = sv::Server::open<TestClient>();
    auto endpoint = Endpoint(IpAddress::Loopback,9999);
    try {
        server.run(endpoint);
        std::cout << "Server is running on " << endpoint.toString() << '\n';
    }
    catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }
    getchar();
}