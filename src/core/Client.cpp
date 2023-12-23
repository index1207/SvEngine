//
// Created by han93 on 2023-12-13.
//

#include "core/Client.hpp"
#include "core/Packet.hpp"

#include "net/Context.hpp"

using namespace sv;

Client::Client() : m_buffer(1024, '\0') {
}

void Client::run(std::unique_ptr<Socket>& sock) {
    m_sock = std::move(sock);
    onConnected();

    auto recvContext = new Context();
    recvContext->completed = bind(&Client::onRecvCompleted, this, std::placeholders::_1);
    recvContext->buffer = m_buffer;
    m_sock->receive(recvContext);
    m_ref = shared_from_this();
}

void Client::onRecvCompleted(Context* context) {
    if(context->length == 0) {
        disconnect();
        return;
    }
    onReceive(context->buffer.subspan(0, context->length), context->length);
    m_sock->receive(context);
}

Client::~Client() {
}

void Client::disconnect() {
    onDisconnected();
    m_ref = nullptr;
}

void Client::send(std::span<char> buffer) {
    m_buffer = std::vector<char>(buffer.begin(), buffer.end());

    auto sendContext = new Context();
    sendContext->completed = bind(&Client::onSendCompleted, this, std::placeholders::_1);
    sendContext->buffer = m_buffer;
    m_sock->send(sendContext);
}

void Client::onSendCompleted(Context* context) {
    onSend(context->buffer, context->length);
    delete context;
}

Socket Client::getSocket() {
    return *m_sock;
}

void Client::send(Packet* packet) {
    packet->write();
    packet->finish();
    send(packet->data());
}