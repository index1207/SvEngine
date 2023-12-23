//
// Created by han93 on 2023-12-23.
//
#include <iostream>
#include <string>
#include <thread>
#include <mutex>

#include "core/Server.hpp"
#include "net/Dns.hpp"
#include "packet.hpp"
#include "core/Client.hpp"

using namespace sv;
using namespace std;

mutex mtx;

class TestServer : public Session {
public:
    TestServer() = default;
    ~TestServer() override = default;
public:
    void onConnected() override
    {
        std::cout << "Connected to server.\n";
        std::string s = "HELLO";
        send(s);
    }
    void onDisconnected() override
    {
        std::cout << "Disconnected.\n";
    }
    void onReceive(std::span<char> buffer, int length) override
    {
        std::lock_guard lock(mtx);
        auto str = std::string(buffer.begin(), buffer.end());
        std::cout << "Recv " << str << '\n';
        send(str);
        this_thread::sleep_for(500ms);
    }
    void onSend(std::span<char> buffer, int length) override
    {
        std::cout << "Send " << length << '\n';
    }
};

int main() {
    auto client = Client::open<TestServer>();
    client.run(Endpoint(IpAddress::Loopback, 9999));

    getchar();
}