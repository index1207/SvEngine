//
// Created by han93 on 2023-12-05.
//

#pragma once
#include <vector>

//#include "net/netcpp.hpp"

#include <net/Socket.hpp>
#include <net/Context.hpp>
#include "Client.hpp"

namespace sv {
    class Server {
        using clientFactory = std::function<sv::Client*()>;
    public:
        Server();
        ~Server();
    public:
        void run(net::Endpoint endpoint, int count = 1);
    public:
        template<class T>
        static T* makeClient() {
            return new T;
        }
        template<class T = sv::Client>
        static Server open()
        {
            Server server;
            server.m_clientFactory = makeClient<T>;
            return server;
        }
    public:
    private:
        void OnAcceptCompleted(net::Context *acceptContext);
    private:
        net::Socket m_listenSock;
        std::vector<net::Context*> m_acceptContexts;
        clientFactory m_clientFactory;
    };
}