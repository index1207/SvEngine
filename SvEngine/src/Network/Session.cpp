//
// Created by han93 on 2023-12-13.
//
#include "pch.h"

#include <Network/Session.hpp>

#include "net/Context.hpp"
#include "net/Exception.hpp"

CREATE_ARENA(Session, 1024)

Session::Session() : m_buffer(1024, '\0'), m_isDisconnected(false) {
}

void Session::Run(std::unique_ptr<Socket> sock) {
    m_sock = std::move(sock);

    m_recvCtx.completed = bind(&Session::OnRecvCompleted, this, std::placeholders::_1, std::placeholders::_2);
    m_recvCtx.buffer = m_buffer;

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
    if(context) delete context;
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

void Session::Send(Packet* packet) {
    packet->Write();
    auto ctx = new Context;
    ctx->completed = std::bind(&Session::OnSendCompleted, this, std::placeholders::_1, std::placeholders::_2);
    ctx->buffer = packet->Data();
    m_sock->send(ctx);
}