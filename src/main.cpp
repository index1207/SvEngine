#include <iostream>
#include <algorithm>

#include "Server.hpp"
#include "net/Dns.hpp"

using namespace net;

class TestClient : public sv::Client
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
        std::cout << "Recv " << buffer.data() << '\n';
        send({buffer.begin(), buffer.begin()+length});
    }
    void onSend(std::span<char> buffer, int length) override
    {
        std::cout << "send " << buffer.data() << '\n';
    }
private:
};

int main() {
    auto server = sv::Server::open<TestClient>();
    auto endpoint = Endpoint(IpAddress::parse("192.168.1.23"),9999);
    try {
        server.run(endpoint);
        std::cout << "Server is running on " << endpoint.toString() << '\n';
    }
    catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }
    getchar();
}