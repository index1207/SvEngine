//
// Created by han93 on 2023-12-05.
//

#include "core/Server.hpp"
#include "core/Session.hpp"

#include "net/Exception.hpp"

#include <functional>
#include <utility>

using namespace sv;

using namespace net;
using namespace std;

Server::Server() {
    m_listenSock.create(Protocol::Tcp);

    m_acceptContexts.reserve(32);
}

void Server::OnAcceptCompleted(net::Context* acceptContext) {
    if(acceptContext->isSuccess)
    {
        auto client = m_clientFactory();
        client->run(acceptContext->acceptSocket);
        client->onConnected();

        acceptContext->acceptSocket = make_unique<Socket>();
    }
    m_listenSock.accept(acceptContext);
}

void Server::run(Endpoint endpoint, int count) {
    if(!m_listenSock.bind(endpoint))
        throw network_error("bind()");
    if(!m_listenSock.listen())
        throw network_error("listen()");

    for(int i = 0; i < count; ++i) {
        auto acceptContext = new Context;
        acceptContext->completed = bind(&Server::OnAcceptCompleted, this, placeholders::_1);

        if(!m_listenSock.accept(acceptContext))
            throw network_error("accept()");

        m_acceptContexts.emplace_back(acceptContext);
    }
}

Server::~Server() {
    for(auto& acceptContext : m_acceptContexts)
        delete acceptContext;
}
