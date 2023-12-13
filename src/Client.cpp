//
// Created by han93 on 2023-12-13.
//

#include "Client.hpp"

using namespace sv;

Client::Client() {
}

void Client::run(std::unique_ptr<Socket>& sock) {
    m_sock = std::move(sock);
    onConnected();
}
