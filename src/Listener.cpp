//
// Created by han93 on 2023-12-05.
//

#include "Listener.hpp"

#include "net/Exception.hpp"

#include <functional>
#include <iostream>

using namespace net;
using namespace std;

Listener::Listener() {
    m_listenSock.create(Protocol::Tcp);

    m_acceptContexts.reserve(128);
}

void Listener::OnAcceptCompleted(net::Context* acceptContext) {
    std::cout << "Connected!\n";

    m_listenSock.accept(acceptContext);
}

void Listener::run(Endpoint endpoint, int count) {
    if(!m_listenSock.bind(endpoint))
        throw network_error("bind()");
    if(!m_listenSock.listen())
        throw network_error("listen()");

    for(int i = 0; i < count; ++i) {
        auto acceptContext = new Context;
        acceptContext->completed = bind(&Listener::OnAcceptCompleted, this, placeholders::_1);

        if(!m_listenSock.accept(acceptContext))
            throw network_error("accept()");

        m_acceptContexts.emplace_back(acceptContext);
    }
}

Listener::~Listener() {
    for(auto& acceptContext : m_acceptContexts)
        delete acceptContext;
}

