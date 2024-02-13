//
// Created by han93 on 2023-12-13.
//
#include "pch.h"

#include "core/Session.hpp"
#include "core/Packet.hpp"
#include "util/Console.hpp"

#include "net/Context.hpp"
#include "net/Exception.hpp"

#include "concurrent_queue.h"

using namespace sv;

Session::Session() : m_buffer(1024, '\0') {
}

void Session::run(std::unique_ptr<Socket> sock) {
    m_sock = std::move(sock);

    m_recvCtx.completed = bind(&Session::onRecvCompleted, this, std::placeholders::_1, std::placeholders::_2);
    m_recvCtx.buffer = m_buffer;

    m_sock->receive(&m_recvCtx);
    m_ref = shared_from_this();
}

void Session::onRecvCompleted(Context *context, bool isSuccess) {
    if(!isSuccess || context->length == 0) {
        disconnect();
        return;
    }
    onReceive(context->buffer.subspan(0, context->length), context->length);
    m_sock->receive(context);
}

void Session::flushQueue()
{
    while (true)
    {
        Packet pkt;
        if (m_sendQue.try_pop(pkt))
            m_sock->send(pkt.data());

        m_sendCount.fetch_sub(1);
        if (m_sendCount == 0)
            return;
    }
}

Session::~Session() {
}

void Session::disconnect() {
    if (!m_isDisconnected)
    {
        m_isDisconnected = true;
        onDisconnected();
        m_ref = nullptr;
    }
}

Socket Session::getSocket() {
    return *m_sock;
}

void Session::send(Packet* packet) {
    auto prevCount = m_sendCount.fetch_add(1);
    packet->write();
    m_sendQue.push(*packet);

    if (prevCount == 0)
    {
        flushQueue();
    }
}