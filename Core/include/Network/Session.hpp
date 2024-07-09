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

class DLLEXPORT Session
{
    friend class Server;
    friend class Client;

    USE_ARENA()
public:
    Session();
    virtual ~Session();
public:
    void Run(std::shared_ptr<Socket> sock);
    Socket GetSocket();
public:
    void Disconnect();
    void SendUnsafe(std::span<char> buffer) const;
    void SendAtomic(std::span<char> buffer);
    void Send(Packet* packet, bool unsafe = false);
public:
    virtual void OnConnected(net::Endpoint) {};
    virtual void OnDisconnected(net::Endpoint) {};
    virtual void OnReceive(std::span<char>, int) {};
    virtual void OnFail(Failure) {};
protected:
    std::shared_ptr<Socket> m_sock;
private:
    void OnRecvCompleted(Context* context, bool isSuccess);
    void OnSendCompleted(Context* context, bool isSuccess);
private:
    std::vector<char> m_buffer;
    net::Context m_recvCtx;
    net::Context m_sendCtx;
    std::atomic<bool> m_isSending;
};