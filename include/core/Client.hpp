//
// Created by han93 on 2023-12-13.
//

#pragma once

#include <vector>
#include "net/Socket.hpp"

using namespace net;

namespace sv {
    class Packet;

    class Client : public std::enable_shared_from_this<Client>
    {
    public:
        Client();
        virtual ~Client();
    public:
        void run(std::unique_ptr<Socket>& sock);
        Socket getSocket();
    public:
        void disconnect();
        void send(std::span<char> buffer);
        void send(Packet* packet);
    public:
        virtual void onConnected() {};
        virtual void onDisconnected() {};
        virtual void onSend(std::span<char> buffer, int length) {};
        virtual void onReceive(std::span<char> buffer, int length) {};
    private:
        void onRecvCompleted(Context* context);
        void onSendCompleted(Context* context);
    private:
        std::unique_ptr<Socket> m_sock;
        std::shared_ptr<Client> m_ref; // TEMP
        std::vector<char> m_buffer;
    };

}