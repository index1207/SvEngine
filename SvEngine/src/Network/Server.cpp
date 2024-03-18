//
// Created by han93 on 2023-12-05.
//
#include "pch.h"

#include "Network/Server.hpp"
#include "Network/Packet.hpp"

#include "net/IoSystem.hpp"
#include "net/Exception.hpp"

using namespace net;
using namespace std;

Server::Server() {
    m_listenSock.create(Protocol::Tcp);
    m_acceptContexts.reserve(32);
}

void Server::OnAcceptCompleted(net::Context* acceptContext, bool isSuccess) {
    if (isSuccess)
    {
        auto client = m_clientFactory();
        client->Run(std::move(acceptContext->acceptSocket));
        acceptContext->acceptSocket = std::make_unique<Socket>(Protocol::Tcp);

        SOCKADDR_IN addr;
        int len = sizeof(addr);
        if (SOCKET_ERROR == getpeername(client->m_sock->getHandle(), reinterpret_cast<SOCKADDR*>(&addr), &len))
        {
            const auto err = WSAGetLastError();
        }
        const auto endpoint = net::Endpoint::parse(addr);
        client->m_sock->setRemoteEndpoint(endpoint);
        client->OnConnected(endpoint);
    }
    m_listenSock.accept(acceptContext);
}

void Server::Run(Endpoint endpoint, int count) {
    if(!m_listenSock.bind(endpoint))
        throw network_error("bind()");
    if(!m_listenSock.listen())
        throw network_error("listen()");

    for(int i = 0; i < count; ++i) {
        auto acceptContext = new Context;
        acceptContext->acceptSocket = std::make_unique<Socket>(Protocol::Tcp);
        acceptContext->completed = std::bind(&Server::OnAcceptCompleted, this, placeholders::_1, placeholders::_2);

        if (!m_listenSock.accept(acceptContext))
            OnAcceptCompleted(acceptContext, false);

        m_acceptContexts.emplace_back(acceptContext);
    }
}

void Server::Cancel()
{
    for (auto* acceptContext : m_acceptContexts) {
        CancelIoEx(reinterpret_cast<HANDLE>(m_listenSock.getHandle()), reinterpret_cast<LPOVERLAPPED>(acceptContext));
        delete acceptContext;
    }
    m_acceptContexts.clear();
}

Server::~Server() {
    Cancel();
}
