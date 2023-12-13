#include <iostream>

#include "Server.hpp"

using namespace net;

class TestClient : public sv::Client {
public:
    TestClient() = default;
public:
    virtual void onConnected() override
    {
        std::cout << "Connected!" << '\n';
    }
    virtual void onDisconnected() override
    {

    }
    virtual void onReceive(std::span<char> buffer) override
    {

    }
    virtual void onSend(std::span<char> buffer) override
    {

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