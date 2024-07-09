//
// Created by han93 on 2023-12-13.
//
#include "pch.h"

#include <Network/Session.hpp>

#include "net/Context.hpp"
#include "net/Exception.hpp"

CREATE_ARENA(Session, 0x1000)

Session::Session() : m_buffer(0x10000, '\0'), m_isSending(false)
{
}

void Session::Run(std::shared_ptr<Socket> sock) {
    m_sock = sock;

    m_recvCtx.completed = bind(&Session::OnRecvCompleted, this, std::placeholders::_1, std::placeholders::_2);
    m_recvCtx.buffer = m_buffer;
    m_sendCtx.completed = bind(&Session::OnSendCompleted, this, std::placeholders::_1, std::placeholders::_2);

    m_sock->receive(&m_recvCtx);
}

void Session::OnRecvCompleted(Context *context, bool isSuccess)
{
    if(!isSuccess || context->length == 0) {
        Disconnect();
        return;
    }
    OnReceive(context->buffer.subspan(0, context->length), context->length);
    m_sock->receive(context);
}

void Session::OnSendCompleted(Context*, bool isSuccess)
{
    if (!isSuccess)
    {
        auto endpoint = m_sock->getRemoteEndpoint();
        if (endpoint.has_value())
            OnDisconnected(endpoint.value());
        delete this;
    }
}

Session::~Session()
{
}

void Session::Disconnect()
{
    OnDisconnected(m_sock->getRemoteEndpoint().value());
    delete this;
}

Socket Session::GetSocket()
{
    return *m_sock;
}

void Session::SendUnsafe(std::span<char> buffer) const
{
    m_sock->send(buffer);
}

void Session::SendAtomic(std::span<char> buffer)
{
    bool expected = false;
    while (m_isSending.compare_exchange_strong(expected, true)) {
        expected = false;
        std::this_thread::yield();
    }
    
    m_sendCtx.buffer = buffer;
    m_sock->send(&m_sendCtx);
    m_isSending.store(false, std::memory_order_release);
}

void Session::Send(Packet* packet, bool unsafe) {
    packet->Write();
    if (unsafe)
        SendUnsafe(packet->Data());
    else
        SendAtomic(packet->Data());
}