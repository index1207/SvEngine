//
// Created by han93 on 2023-12-16.
//

#include <winsock2.h>

#include "core/Packet.hpp"

sv::Packet::Packet(unsigned short id, int reserve) : m_buffer(4, 0), m_id(0), m_size(0) {
    m_buffer.reserve(reserve);
    for (int i = sizeof(unsigned short) - 1; i >= 0; --i)
        m_buffer[sizeof(unsigned short) - i - 1] = id >> 8 * i & 0xFF;
}

std::vector<char>& sv::Packet::data() {
    return m_buffer;
}

sv::Packet& sv::Packet::operator<<(unsigned char data) {
    m_buffer.push_back(static_cast<char>(data));
    return *this;
}

sv::Packet &sv::Packet::operator<<(char data) {
    m_buffer.push_back(data);
    return *this;
}

sv::Packet &sv::Packet::operator<<(unsigned short data) {
    auto t = static_cast<unsigned short>(data);
    for(int i = sizeof(t)-1; i >= 0; --i)
        m_buffer.push_back((t>>8*i)&0xFF);
    return *this;
}

sv::Packet &sv::Packet::operator<<(unsigned int data) {
    for(int i = sizeof(data)-1; i >= 0; --i)
        m_buffer.push_back((data>>8*i)&0xFF);
    return *this;
}

sv::Packet &sv::Packet::operator<<(unsigned long long int data) {
    auto t = data;
    for(int i = sizeof(t)-1; i >= 0; --i)
        m_buffer.push_back((t>>8*i)&0xFF);
    return *this;
}

sv::Packet &sv::Packet::operator<<(short data) {
    auto t = static_cast<unsigned short>(data);
    for(int i = sizeof(t)-1; i >= 0; --i)
        m_buffer.push_back((t>>8*i)&0xFF);
    return *this;
}

sv::Packet &sv::Packet::operator<<(int data) {
    for(int i = sizeof(data)-1; i >= 0; --i)
        m_buffer.push_back((data>>8*i)&0xFF);
    return *this;
}

sv::Packet &sv::Packet::operator<<(long data) {
    for(int i = sizeof(data)-1; i >= 0; --i)
        m_buffer.push_back((data>>8*i)&0xFF);
    return *this;
}

sv::Packet &sv::Packet::operator<<(float data) {
    char buffer[sizeof(float)] = "";
    std::memcpy(buffer, &data, sizeof(float));
    m_buffer.insert(m_buffer.end(), buffer, buffer+sizeof(float));
    return *this;
}

sv::Packet &sv::Packet::operator<<(double data) {
    char buffer[sizeof(double)] = "";
    std::memcpy(buffer, &data, sizeof(double));
    m_buffer.insert(m_buffer.end(), buffer, buffer+sizeof(double));
    return *this;
}

sv::Packet &sv::Packet::operator<<(std::string_view data) {
    *this << static_cast<unsigned short>(data.length());
    m_buffer.insert(m_buffer.end(), data.begin(), data.end());
    return *this;
}

sv::Packet &sv::Packet::operator<<(bool data) {
    return *this << static_cast<unsigned char>(data);
}

sv::Packet &sv::Packet::operator<<(unsigned long data) {
    for(int i = sizeof(data)-1; i >= 0; --i)
        m_buffer.push_back((data>>8*i)&0xFF);
    return *this;
}

void sv::Packet::finish() {
    auto size = static_cast<unsigned short>(m_buffer.size()-2);
    for (int i = sizeof(size) - 1; i >= 0; --i)
        m_buffer[sizeof(unsigned short) - i + 1] = size >> 8 * i & 0xFF;
}

void sv::Packet::parse(std::span<char> buffer) {
    m_buffer = std::vector(buffer.begin(), buffer.end());
    read();
}

sv::Packet* sv::Packet::parseFrom(std::span<char> buffer)
{
    Packet pk(0);
    pk.parse(buffer);
    return &pk;
}

void sv::Packet::read() {
    *this >> m_id >> m_size;
}
