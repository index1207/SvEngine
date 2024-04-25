#include "pch.h"
#include "Network/Client.hpp"
#include "net/Exception.hpp"

#include <net/Socket.hpp>
#include <net/Context.hpp>

Client::Client() {
    m_sock.create(Protocol::Tcp);
}

Client::~Client() {
}

void Client::Run(Endpoint endpoint) {
    m_sock.setLocalEndpoint(endpoint);

    auto connectContext = new Context;
    connectContext->endpoint = endpoint;
    connectContext->completed = bind(&Client::OnConnectCompleted, this, std::placeholders::_1, std::placeholders::_2);
    m_sock.connect(connectContext);
}

void Client::OnConnectCompleted(Context* context, bool isSuccess) {
    auto client = m_serverFactory();
    if (isSuccess) {
        client->Run(std::make_unique<Socket>(m_sock));

    //    SOCKADDR_IN addr;
    //    int len = sizeof(addr);
    //    if (SOCKET_ERROR == getpeername(client->m_sock->getHandle(), reinterpret_cast<SOCKADDR*>(&addr), &len))
    //        throw net::network_error("getpeername()");
        client->OnConnected(Endpoint());
    }
    else
        client->OnFail(Failure::Connect);
}
