//
// Created by han93 on 2023-12-13.
//

#include "core/Session.hpp"
#include "core/Packet.hpp"

#include "net/Context.hpp"
#include "util/Console.hpp"

#include "DefinedPacket.hpp"
#include <PacketHandler.hpp>

using namespace sv;

Session::Session() : m_buffer(1024, '\0') {
}

void Session::run(std::unique_ptr<Socket>& sock) {
    m_sock = std::move(sock);

    m_recvCtx.completed = bind(&Session::onRecvCompleted, this, std::placeholders::_1, std::placeholders::_2);
    m_recvCtx.buffer = m_buffer;

    m_sock->receive(&m_recvCtx);
    m_ref = shared_from_this();
}

void sv::Session::onReceive(char* buffer, int length)
{
    auto pk = Packet::parseFrom(std::span<char>(buffer, length));
    gen::PacketHandler::onReceivePacket(static_cast<gen::PacketId>(pk->getId()), pk);
}

void Session::onRecvCompleted(Context *context, bool isSuccess) {
    if(!isSuccess || context->length == 0) {
        disconnect();
        return;
    }

    onReceive(context->buffer.data(), context->length);
    m_sock->receive(context);
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

void Session::send(std::span<char> buffer) {
    auto sendCtx = new Context;
    sendCtx->completed = std::bind(&Session::onSendCompleted, this, std::placeholders::_1, std::placeholders::_2);
    sendCtx->buffer = buffer;
    try {
        m_sock->send(sendCtx);
    }
    catch (std::exception& e) {
        Console::Log(e.what());
    }
}

void Session::onSendCompleted(Context *context, bool isSuccess) {
    if (!isSuccess || context->length == 0) {
        delete context;
        disconnect();
        return;
    }
    onSend(context->length);
    delete context;
}

Socket Session::getSocket() {
    return *m_sock;
}

void Session::send(Packet* packet) {
    packet->write();
    packet->finish();
    send(packet->data());
}