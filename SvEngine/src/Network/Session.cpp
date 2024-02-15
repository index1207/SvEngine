//
// Created by han93 on 2023-12-13.
//
#include "pch.h"

#include <Network/Session.hpp>

#include "net/Context.hpp"
#include "net/Exception.hpp"

Session::Session() : m_buffer(1024, '\0') {
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
    onReceive(context->buffer.subspan(0, context->length), context->length);
    m_sock->receive(context);
}

void Session::FlushQueue()
{
    while (true)
    {
        Packet pkt;
        if (m_sendQue.try_pop(pkt))
            m_sock->send(pkt.Data());

        m_sendCount.fetch_sub(1);
        if (m_sendCount == 0)
            return;
    }
}

Session::~Session() {
}

void Session::Disconnect() {
    if (!m_isDisconnected)
    {
        m_isDisconnected = true;
        onDisconnected();
        m_ref = nullptr;
    }
}

Socket Session::GetSocket() {
    return *m_sock;
}

void Session::Send(Packet* packet) {
    auto prevCount = m_sendCount.fetch_add(1);
    packet->Write();
    m_sendQue.push(*packet);

    if (prevCount == 0)
    {
        FlushQueue();
    }
}