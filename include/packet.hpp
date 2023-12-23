#pragma once
#include <core/Packet.hpp>
#include <vector>

enum PacketId {
    CHAT
};

class Chat
        : public sv::Packet {
public:
    Chat() : sv::Packet(PacketId::CHAT) {
    }
    ~Chat() {

    }
public:
    void read() override
    {
        Packet::read();
        *this >> str;
    }
    void write() override
    {
        *this << str;
        finish();
    }
public:
    std::string str;
};

