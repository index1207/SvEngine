//
// Created by han93 on 2023-12-13.
//
#include "pch.h"

#include <Network/Session.hpp>

#include "net/Context.hpp"
#include "net/Exception.hpp"

CREATE_ARENA(Session, 0x1000)

Session::Session() : m_buffer(0x10000, '\0'), m_isDisconnected(false), m_flushSend(false) {
}

void Session::Run(std::shared_ptr<Socket> sock) {
    m_sock = sock;

    m_recvCtx.completed = bind(&Session::OnRecvCompleted, this, std::placeholders::_1, std::placeholders::_2);
    m_recvCtx.buffer = m_buffer;
    m_sendCtx.completed = bind(&Session::OnSendCompleted, this, std::placeholders::_1, std::placeholders::_2);

    m_sock->receive(&m_recvCtx);
    m_ref = shared_from_this();
}

void Session::OnRecvCompleted(Context *context, bool isSuccess) {
    if(!isSuccess || context->length == 0) {
        Disconnect();
        return;
    }
    OnReceive(context->buffer.subspan(0, context->length), context->length);
    m_sock->receive(context);
}

void Session::OnSendCompleted(Context* context, bool isSuccess)
{
    m_sendCtx.sendBuffer.clear();
    m_flushSend.store(false);
}

Session::~Session() {
}

void Session::Disconnect() {
    if (!m_isDisconnected.exchange(true))
    {
        OnDisconnected(m_sock->getRemoteEndpoint().value());
        m_ref = nullptr;
    }
}

Socket Session::GetSocket() {
    return *m_sock;
}

void Session::Send(std::span<char> buffer)
{
    m_sendCtx.sendBuffer.push_back(buffer);
    if (!m_flushSend.exchange(true))
    {
        m_sock->send(&m_sendCtx);
    }
}

void Session::Send(Packet* packet) {
    packet->Write();
    Send(packet->Data());
}
