#pragma once

#include <net/Endpoint.hpp>

#include "Session.hpp"

class SVENGINE_API Client {
    using ServerFactory = std::function<Session*()>;

    Client();
public:
    ~Client();
public:
    void Run(net::Endpoint endpoint);
public:
    template<class T = Session>
    static inline std::unique_ptr<Client> Open()
    {
        auto client = std::unique_ptr<Client>(new Client);
        client->m_serverFactory = [] {
            return new T();
        };
        return std::move(client);
    }
private:
    void OnConnectCompleted(Context* context, bool isSuccess);
private:
    Socket m_sock;
    ServerFactory m_serverFactory;
};