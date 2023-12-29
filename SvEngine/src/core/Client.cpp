//
// Created by han93 on 2023-12-23.
//

#include "core/Client.hpp"
#include "net/Exception.hpp"

#include <net/Context.hpp>

sv::Client::Client() {
    m_sock.create(Protocol::Tcp);
}

sv::Client::~Client() {
    m_sock.close();
}

void sv::Client::run(net::Endpoint endpoint) {
    m_sock.setLocalEndpoint(endpoint);

    auto connectContext = new Context;
    connectContext->endpoint = std::make_unique<Endpoint>(endpoint);
    connectContext->completed = bind(&Client::onConnectCompleted, this, std::placeholders::_1, std::placeholders::_2);
    m_sock.connect(connectContext);
}

void sv::Client::onConnectCompleted(Context *context, bool isSuccess) {
    if(isSuccess) {
        auto client = m_serverFactory();
        auto sock = std::make_unique<Socket>(m_sock);
        client->run(sock);
        client->onConnected();
    }
    else
        printf("Can't connect to server.\n");
}
