//
// Created by han93 on 2023-12-16.
//

#pragma once

#include <vector>
#include <string_view>

namespace sv {
    class Packet {
    public:
        Packet(unsigned short id, int reserve = 1024);
    public:
        Packet& operator<<(unsigned char data);
        Packet& operator<<(unsigned short data);
        Packet& operator<<(unsigned int data);
        Packet& operator<<(unsigned long data);
        Packet& operator<<(unsigned long long data);
        Packet& operator<<(char data);
        Packet& operator<<(short data);
        Packet& operator<<(int data);
        Packet& operator<<(long data);
        Packet& operator<<(float data);
        Packet& operator<<(double data);
        Packet& operator<<(std::wstring_view data);
        Packet& operator<<(bool data);
    public:
        std::vector<char>& data();
    private:
        std::vector<char> m_buffer;
    };
}