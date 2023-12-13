#include <iostream>
#include <algorithm>

#include "Server.hpp"

using namespace net;

class TestClient : public sv::Client {
public:
    TestClient() = default;
    ~TestClient() override = default;
public:
    void onConnected() override
    {
        std::cout << "Connected!" << '\n';
    }
    void onDisconnected() override
    {
        std::cout << "Disconnected!" << '\n';
    }
    void onReceive(std::span<char> buffer, int length) override
    {
        std::cout << "Recv " << buffer.data() << '\n';
        send({buffer.begin(), buffer.begin()+length});
    }
    void onSend(std::span<char> buffer, int length) override
    {
        std::cout << "send " << length << '\n';
    }
private:
};

int main() {
    auto server = sv::Server::open<TestClient>();
    try {
        server.run(Endpoint(IpAddress::Loopback, 9999));
    }
    catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }
    getchar();
}