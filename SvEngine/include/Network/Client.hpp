#pragma once

#include <net/Endpoint.hpp>

#include "Session.hpp"

class Client {
    using ServerFactory = std::function<std::shared_ptr<Session>()>;

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
            return MakeShared<T>();
        };
        return client;
    }
private:
    void OnConnectCompleted(Context* context, bool isSuccess);
private:
    Socket m_sock;
    ServerFactory m_serverFactory;
};