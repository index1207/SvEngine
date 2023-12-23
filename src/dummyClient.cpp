//
// Created by han93 on 2023-12-23.
//
#include <iostream>
#include <string>

#include "core/Server.hpp"
#include "net/Dns.hpp"
#include "packet.hpp"
#include "core/Client.hpp"

using namespace sv;

class TestServer : public Session {
public:
    TestServer() = default;
    ~TestServer() override = default;
public:
    void onConnected() override
    {
        std::cout << "Connected to server.\n";
    }
    void onDisconnected() override
    {

    }
    void onReceive(std::span<char> buffer, int length) override
    {

    }
    void onSend(std::span<char> buffer, int length) override
    {

    }
};

int main() {
    auto client = Client::open<TestServer>();
    client.run(Endpoint(IpAddress::Loopback, 9999));

    getchar();
}