#pragma once

#include <vector>
#include <functional>

#include "net/Context.hpp"

using namespace net;

namespace sv {
    class Packet;

    enum class Failure
    {
        None,
        Connect
    };

    class Session : public std::enable_shared_from_this<Session>
    {
        friend class Server;
        using ServerFactory = std::function<std::shared_ptr<Session>()>;
    public:
        Session();
        virtual ~Session();
    public:
        void Run(std::unique_ptr<Socket> sock);
        Socket GetSocket();
    public:
        void Disconnect();
        void Send(std::span<char> buffer);
    public:
        virtual void OnConnected() {};
        virtual void OnDisconnected() {};
        virtual void OnReceive(std::span<char> buffer, int length) {};
        virtual void OnFail(Failure cause) {};
    protected:
        std::unique_ptr<Socket> m_sock;
    private:
        void OnRecvCompleted(Context *context, bool isSuccess);
    private:
        std::shared_ptr<Session> m_ref; // TEMP
        std::vector<char> m_buffer;
        net::Context m_recvCtx;
        std::atomic<bool> m_isDisconnected;
    };
}