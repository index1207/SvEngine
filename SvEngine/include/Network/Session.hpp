//
// Created by han93 on 2023-12-13.
//

#pragma once

using namespace net;

#include <Network/Packet.hpp>

class Packet;

enum class Failure
{
    None,
    Connect
};

class Session : public std::enable_shared_from_this<Session>
{
    friend class Server;
    friend class Client;
    using ServerFactory = std::function<std::shared_ptr<Session>()>;
public:
    Session();
    virtual ~Session();
public:
    void Run(std::unique_ptr<Socket> sock);
    Socket GetSocket();
public:
    void Disconnect();
    void Send(Packet* packet);
public:
    virtual void OnConnected(net::Endpoint) {};
    virtual void OnDisconnected(net::Endpoint) {};
    virtual void OnReceive(std::span<char>, int) {};
    virtual void OnFail(Failure) {};
protected:
    std::unique_ptr<Socket> m_sock;
private:
    void OnRecvCompleted(Context* context, bool isSuccess);
    void FlushQueue();
private:
    ConcurrencyQueue<Packet> m_sendQue;
    std::atomic<int> m_sendCount;

    std::shared_ptr<Session> m_ref; // TEMP

    std::vector<char> m_buffer;
    net::Context m_recvCtx;

    std::atomic<bool> m_isDisconnected;
};