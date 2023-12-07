#include <iostream>

#include "Listener.hpp"

using namespace net;

int main() {
    try {
        Listener listener;
        listener.run(Endpoint(IpAddress::Loopback, 9999), 1);
    }
    catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }
    while(true) {}
}