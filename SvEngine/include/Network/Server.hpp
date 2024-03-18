//
// Created by han93 on 2023-12-05.
//

#pragma once

#include <Network/Session.hpp>

class Server {
    friend class Session;
    using ClientFactory = std::function<std::shared_ptr<Session>()>;
    Server();
public:
    ~Server();
public:
    void Run(net::Endpoint endpoint, int count = 1);
    void Cancel();
public:
    template<class T>
    static inline std::shared_ptr<Server> Open()
    {
        auto server = std::shared_ptr<Server>(new Server);
        server->m_clientFactory = [] {
            return std::make_shared<T>();
            };
        return server;
    }
private:
    void OnAcceptCompleted(net::Context* acceptContext, bool isSuccess);
private:
    net::Socket m_listenSock;
    std::vector<net::Context*> m_acceptContexts;
    ClientFactory m_clientFactory;
};