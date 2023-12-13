//
// Created by han93 on 2023-12-13.
//

#pragma once

#include <net/Socket.hpp>

using namespace net;

namespace sv {

    class Client : public std::enable_shared_from_this<Client>
    {
    public:
        Client();
        virtual ~Client();
    public:
        void run(std::unique_ptr<Socket>& sock);
    public:
        void disconnect();
    public:
        virtual void onConnected() {};
        virtual void onDisconnected() {};
        virtual void onSend(std::span<char> buffer) {};
        virtual void onReceive(std::span<char> buffer, int length) {};
    private:
        void onRecvCompleted(Context* context);
    private:
        std::unique_ptr<Socket> m_sock;
        std::shared_ptr<Client> m_ref;
    };

}