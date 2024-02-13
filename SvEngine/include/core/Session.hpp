//
// Created by han93 on 2023-12-13.
//

#pragma once

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
        using serverFactory = std::function<std::shared_ptr<Session>()>;
    public:
        Session();
        virtual ~Session();
    public:
        void run(std::unique_ptr<Socket> sock);
        Socket getSocket();
    public:
        void disconnect();
        void send(Packet* packet);
    public:
        virtual void onConnected() {};
        virtual void onDisconnected() {};
        virtual void onReceive(std::span<char>, int) {};
        virtual void onFail(Failure) {};
    protected:
        std::unique_ptr<Socket> m_sock;
    private:
        void onRecvCompleted(Context *context, bool isSuccess);
        void flushQueue();
    private:
        concurrency::concurrent_queue<sv::Packet> m_sendQue;
        std::atomic<int> m_sendCount;
        std::shared_ptr<Session> m_ref; // TEMP
        std::vector<char> m_buffer;
        net::Context m_recvCtx;
        std::atomic<bool> m_isDisconnected;
    };
}