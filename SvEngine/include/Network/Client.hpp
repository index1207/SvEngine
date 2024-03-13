#pragma once

#include <net/Endpoint.hpp>

#include "Session.hpp"

class Client {
    using serverFactory = std::function<std::shared_ptr<Session>()>;

    Client();
public:
    ~Client();
public:
    void Run(net::Endpoint endpoint);
public:
    template<class T = Session>
    static inline std::shared_ptr<Client> Open()
    {
        auto client = std::shared_ptr<Client>(new Client);
        client->m_serverFactory = [] {
            return std::make_shared<T>();
            };
        return client;
    }
private:
    void onConnectCompleted(Context* context, bool isSuccess);
private:
    Socket m_sock;
    serverFactory m_serverFactory;
};