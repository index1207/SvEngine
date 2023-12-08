//
// Created by han93 on 2023-12-05.
//

#pragma once
#include <vector>

//#include "net/netcpp.hpp"

#include <net/Socket.hpp>
#include <net/Context.hpp>

class Listener {
public:
    Listener();
    ~Listener();
public:
    void run(net::Endpoint endpoint, int count = 1);
public:
private:
    void OnAcceptCompleted(net::Context* acceptContext);
private:
    std::vector<net::Context*> m_acceptContexts;
    net::Socket m_listenSock;
};