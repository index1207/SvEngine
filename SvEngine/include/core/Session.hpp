//
// Created by han93 on 2023-12-13.
//

#pragma once

#include <vector>
#include <functional>

#include "net/Socket.hpp"
#include "net/Context.hpp"

using namespace net;

namespace sv {
    class Packet;
    class Session : public std::enable_shared_from_this<Session>
    {
        friend class Server;
        using serverFactory = std::function<std::shared_ptr<Session>()>;
    public:
        Session();
        virtual ~Session();
    public:
        void run(std::unique_ptr<Socket> sock);
        Socket getSocket();
    public:
        void disconnect();
        void send(std::span<char> buffer);
        void send(Packet* packet);
    public:
        virtual void onConnected() {};
        virtual void onDisconnected() {};
        virtual void onReceive(std::span<char> buffer, int length);
    protected:
        std::unique_ptr<Socket> m_sock;
    private:
        void onRecvCompleted(Context *context, bool isSuccess);
    private:
        std::shared_ptr<Session> m_ref; // TEMP
        std::vector<char> m_buffer;
        net::Context m_recvCtx;
        std::atomic<bool> m_isDisconnected;
        std::mutex m_mtx;
    };

}