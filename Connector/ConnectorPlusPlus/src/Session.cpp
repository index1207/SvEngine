#include "Session.hpp"

#include "net/Context.hpp"
#include "net/Exception.hpp"

using namespace sv;

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

void Session::Send(std::span<char> buffer) {
    std::lock_guard lock(m_mtx);
    if (!m_sock->send(buffer)) {
        throw net::network_error("send()");
    }
}

Socket Session::GetSocket() {
    return *m_sock;
}