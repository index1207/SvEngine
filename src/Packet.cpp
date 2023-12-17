//
// Created by han93 on 2023-12-16.
//

#include <winsock2.h>

#include "Packet.hpp"

sv::Packet::Packet(unsigned short id, int reserve) : m_buffer(4, 0) {
    m_buffer.reserve(reserve);
    for(int i = sizeof(id)-1; i >= 0; --i)
        m_buffer[i-sizeof(id)+1] = (id>>8*i)&0xFF;
}

std::vector<char>& sv::Packet::data() {
    auto size = static_cast<unsigned short>(m_buffer.size());
    for(int i = sizeof(size)-1; i >= 0; --i)
        m_buffer[sizeof(size)-i+1] = (size>>8*i)&0xFF;
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
    auto t = static_cast<unsigned short>(htons(static_cast<unsigned short>(data)));
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
    auto t = htonll(data);
    for(int i = sizeof(t)-1; i >= 0; --i)
        m_buffer.push_back((t>>8*i)&0xFF);
    return *this;
}

sv::Packet &sv::Packet::operator<<(short data) {
    auto t = htons(static_cast<unsigned short>(data));
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
    auto t = htonl(static_cast<unsigned long>(data));
    for(int i = sizeof(t)-1; i >= 0; --i)
        m_buffer.push_back((t>>8*i)&0xFF);
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

sv::Packet &sv::Packet::operator<<(std::wstring_view data) {

    return *this;
}

sv::Packet &sv::Packet::operator<<(bool data) {
    return *this << static_cast<unsigned char>(data);
}

sv::Packet &sv::Packet::operator<<(unsigned long data) {
    auto t = htonl(data);
    for(int i = sizeof(t)-1; i >= 0; --i)
        m_buffer.push_back((t>>8*i)&0xFF);
    return *this;
}
