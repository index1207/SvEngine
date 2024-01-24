//
// Created by han93 on 2023-12-05.
//

#pragma once
#include <vector>

#include "net/Socket.hpp"
#include "net/Context.hpp"
#include "Session.hpp"

namespace sv {
    class Server {
        friend class Session;
        using clientFactory = std::function<std::shared_ptr<Session>()>;
        Server();
    public:
        ~Server();
    public:
        void run(net::Endpoint endpoint, int count = 1);
        void loop();
        void cancel();
    public:
        template<class T = sv::Session>
        static inline Server open()
        {
            Server server;
            server.m_clientFactory = []{
                return std::make_shared<T>();
            };
            return server;
        }
    private:
        void onAcceptCompleted(net::Context *acceptContext, bool isSuccess);
    private:
        net::Socket m_listenSock;
        std::vector<net::Context*> m_acceptContexts;
        clientFactory m_clientFactory;
    };
}