//
// Created by han93 on 2023-12-13.
//

#include "Client.hpp"

#include <net/Context.hpp>

using namespace sv;

Client::Client() {
}

void Client::run(std::unique_ptr<Socket>& sock) {
    m_sock = std::move(sock);
    onConnected();

    auto recvContext = new Context();
    std::vector<char> v(128);
    recvContext->completed = bind(&Client::onRecvCompleted, this, std::placeholders::_1);
    recvContext->buffer = v;
    m_sock->receive(recvContext);
    m_ref = shared_from_this();
}

void Client::onRecvCompleted(Context* context) {
    if(context->length == 0) {
        disconnect();
        return;
    }
    onReceive(context->buffer, context->length);
    m_sock->receive(context);
}

Client::~Client() {
}

void Client::disconnect() {
    onDisconnected();
    m_ref = nullptr;
}
