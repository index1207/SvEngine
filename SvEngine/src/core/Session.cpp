//
// Created by han93 on 2023-12-13.
//

#include "core/Session.hpp"
#include "core/Packet.hpp"

#include "net/Context.hpp"
#include "util/Console.hpp"

#include "generated/PacketHandler.hpp"

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

void sv::Session::onReceivePacket(std::span<char> buffer, int length)
{
    if (length <= 2)
        return;

    gen::PacketId id;
    std::memcpy(&id, buffer.data(), sizeof(unsigned short));

    gen::PacketHandler::onReceivePacket(this, id, Packet::parseFrom(buffer).get());
}

void Session::onRecvCompleted(Context *context, bool isSuccess) {
    if(!isSuccess || context->length == 0) {
        disconnect();
        return;
    }
    onReceive(context->buffer.subspan(0, context->length), context->length);
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
    std::lock_guard lock(m_mtx);
    m_sock->send(buffer);
}

Socket Session::getSocket() {
    return *m_sock;
}

void Session::send(Packet* packet) {
    packet->write();
    packet->finish();
    send(packet->data());
}