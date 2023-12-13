//
// Created by han93 on 2023-12-13.
//

#pragma once

#include <net/Socket.hpp>

using namespace net;

namespace sv {

    class Client {
    public:
        Client();
    public:
        void run(std::unique_ptr<Socket>& sock);
    public:
        virtual void onConnected() {};
        virtual void onDisconnected() {};
        virtual void onSend(std::span<char> buffer) {};
        virtual void onReceive(std::span<char> buffer) {};
    private:
        std::unique_ptr<Socket> m_sock;
    };

}