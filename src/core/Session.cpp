//
// Created by han93 on 2023-12-13.
//

#include "core/Session.hpp"
#include "core/Packet.hpp"

#include <mutex>

#include "net/Context.hpp"

using namespace sv;

std::mutex mtx;

Session::Session() : m_buffer(1024, '\0') {
}

void Session::run(std::unique_ptr<Socket>& sock) {
    m_sock = std::move(sock);

    auto recvContext = new Context();
    recvContext->completed = bind(&Session::onRecvCompleted, this, std::placeholders::_1);
    recvContext->buffer = m_buffer;
    m_sock->receive(recvContext);
    m_ref = shared_from_this();
}

void Session::onRecvCompleted(Context* context) {
    if(context->length == 0) {
        disconnect();
        return;
    }
    onReceive(context->buffer.subspan(0, context->length), context->length);
    m_sock->receive(context);
}

Session::~Session() {
}

void Session::disconnect() {
    onDisconnected();
    m_ref = nullptr;
}

void Session::send(std::span<char> buffer) {
    auto sendContext = new Context();
    sendContext->completed = bind(&Session::onSendCompleted, this, std::placeholders::_1);

    std::lock_guard lock(mtx);
    m_buffer = std::vector<char>(buffer.begin(), buffer.end());
    sendContext->buffer = m_buffer;
    m_sock->send(sendContext);
}

void Session::onSendCompleted(Context* context) {
    onSend(context->buffer, context->length);
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
