#include "Client.hpp"
#include "net/Exception.hpp"

#include <net/Context.hpp>

sv::Client::Client() {
    m_sock.create(Protocol::Tcp);
}

sv::Client::~Client() {
}

void sv::Client::Run(Endpoint endpoint) {
    m_sock.setLocalEndpoint(endpoint);

    auto connectContext = new Context;
    connectContext->endpoint = std::make_unique<Endpoint>(endpoint);
    connectContext->completed = bind(&Client::onConnectCompleted, this, std::placeholders::_1, std::placeholders::_2);
    m_sock.Connect(connectContext);
}

void sv::Client::onConnectCompleted(Context *context, bool isSuccess) {
    auto client = m_serverFactory();
    if (isSuccess) {
        client->Run(std::make_unique<Socket>(m_sock));
        client->onConnected();
    }
    else
        client->onFail(Failure::Connect);
}
