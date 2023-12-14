//
// Created by han93 on 2023-12-05.
//

#pragma once
#include <vector>

#include <net/Socket.hpp>
#include <net/Context.hpp>
#include "Client.hpp"

namespace sv {
    class Server {
        using clientFactory = std::function<std::shared_ptr<Client>()>;
    public:
        Server();
        ~Server();
    public:
        void run(net::Endpoint endpoint, int count = 1);
    public:
        template<class T = sv::Client>
        static inline Server open()
        {
            Server server;
            server.m_clientFactory = []{
                return std::make_shared<T>();
            };
            return server;
        }
    private:
        void OnAcceptCompleted(net::Context *acceptContext);
    private:
        net::Socket m_listenSock;
        std::vector<net::Context*> m_acceptContexts;
        clientFactory m_clientFactory;
    };
}