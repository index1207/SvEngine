//
// Created by han93 on 2023-12-16.
//
#include "pch.h"

#include <winsock2.h>

#include "core/Packet.hpp"
#include <core/Session.hpp>

using namespace sv;

Packet::Packet(unsigned short id, int reserve) : m_buffer(4, 0), m_id(0), m_size(0) {
    m_buffer.reserve(reserve);
    for (int i = sizeof(unsigned short) - 1; i >= 0; --i)
        m_buffer[sizeof(unsigned short) - i - 1] = id >> 8 * i & 0xFF;
}

std::vector<char>& Packet::data()
{
    return m_buffer;
}

void Packet::finish()
{
    auto size = static_cast<unsigned short>(m_buffer.size() - 4);
    for (int i = sizeof(size) - 1; i >= 0; --i)
        m_buffer[sizeof(unsigned short) - i + 1] = size >> 8 * i & 0xFF;
}

void Packet::parse(std::span<char> buffer)
{
    m_buffer = std::vector(buffer.begin(), buffer.end());
    read();
}

void Packet::read()
{
    *this >> m_id >> m_size;
}

Packet& Packet::operator<<(unsigned char data) {
    m_buffer.push_back(static_cast<char>(data));
    return *this;
}

Packet& Packet::operator<<(unsigned short data) {
    for(int i = sizeof(data)-1; i >= 0; --i)
        m_buffer.push_back((data>>8*i)&0xFF);
    return *this;
}

Packet& Packet::operator<<(unsigned int data) {
    for(int i = sizeof(data)-1; i >= 0; --i)
        m_buffer.push_back((data>>8*i)&0xFF);
    return *this;
}

Packet& Packet::operator<<(unsigned long data) {
    for(int i = sizeof(data)-1; i >= 0; --i)
        m_buffer.push_back((data>>8*i)&0xFF);
    return *this;
}

Packet& Packet::operator<<(unsigned long long data) {
    for(int i = sizeof(data)-1; i >= 0; --i)
        m_buffer.push_back((data >>8*i)&0xFF);
    return *this;
}

Packet& Packet::operator<<(bool data) {
    return *this << static_cast<unsigned char>(data);
}

Packet& Packet::operator<<(char data) {
    m_buffer.push_back(data);
    return *this;
}

Packet& Packet::operator<<(short data) {
    for(int i = sizeof(data)-1; i >= 0; --i)
        m_buffer.push_back((data>>8*i)&0xFF);
    return *this;
}

Packet& Packet::operator<<(int data) {
    for(int i = sizeof(data)-1; i >= 0; --i)
        m_buffer.push_back((data>>8*i)&0xFF);
    return *this;
}

Packet& Packet::operator<<(long data) {
    for(int i = sizeof(data)-1; i >= 0; --i)
        m_buffer.push_back((data>>8*i)&0xFF);
    return *this;
}

Packet& Packet::operator<<(long long data) {
    for (int i = sizeof(data) - 1; i >= 0; --i)
        m_buffer.push_back((data>>8*i)&0xFF);
    return *this;
}

Packet& Packet::operator<<(float data) {
    char buffer[sizeof(float)] = "";
    std::memcpy(buffer, &data, sizeof(float));
    m_buffer.insert(m_buffer.end(), buffer, buffer+sizeof(float));
    return *this;
}

Packet& Packet::operator<<(double data) {
    char buffer[sizeof(double)] = "";
    std::memcpy(buffer, &data, sizeof(double));
    m_buffer.insert(m_buffer.end(), buffer, buffer+sizeof(double));
    return *this;
}

Packet& Packet::operator<<(std::string_view data) {
    *this << static_cast<unsigned short>(data.length());
    m_buffer.insert(m_buffer.end(), data.begin(), data.end());
    return *this;
}

Packet& Packet::operator>>(unsigned char& data)
{
    data = m_buffer.front();
    m_buffer.erase(m_buffer.begin());
    return *this;
}

Packet& Packet::operator>>(unsigned short& data)
{
    std::memcpy(&data, m_buffer.data(), sizeof(data));
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + sizeof(data));

    data = ntohs(data);
    return *this;
}

Packet& Packet::operator>>(unsigned int& data)
{
    std::memcpy(&data, m_buffer.data(), sizeof(data));
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + sizeof(data));
    data = static_cast<unsigned int>(ntohl(static_cast<u_long>(data)));
    data = ntohl(data);
    return *this;
}

Packet& Packet::operator>>(unsigned long& data)
{
    std::memcpy(&data, m_buffer.data(), sizeof(data));
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + sizeof(data));
    data = ntohl(data);
    return *this;
}

Packet& Packet::operator>>(unsigned long long& data)
{
    std::memcpy(&data, m_buffer.data(), sizeof(data));
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + sizeof(data));
    data = ntohll(data);
    return *this;
}

Packet& Packet::operator>>(bool& data)
{
    unsigned char t;
    *this >> t;
    data = static_cast<bool>(t);

    return *this;
}

Packet& Packet::operator>>(char& data)
{
    unsigned char t;
    *this >> t;
    data = static_cast<char>(t);

    return *this;
}

Packet& Packet::operator>>(short& data)
{
    unsigned short t;
    *this >> t;
    data = static_cast<short>(t);
    return *this;
}

Packet& Packet::operator>>(long& data)
{
    unsigned long t;
    *this >> t;
    data = static_cast<long>(t);
    return *this;
}

Packet& Packet::operator>>(long long& data)
{
    unsigned long long t;
    *this >> t;
    data = static_cast<long long>(t);
    return *this;
}

Packet& Packet::operator>>(int& data)
{
    unsigned int t = 0;
    *this >> t;
    data = static_cast<int>(t);
    return *this;
}

Packet& Packet::operator>>(float& data)
{
    std::memcpy(&data, m_buffer.data(), sizeof(data));
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + sizeof(data));
    return *this;
}

Packet& Packet::operator>>(double& data)
{
    std::memcpy(&data, m_buffer.data(), sizeof(data));
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + sizeof(data));
    return *this;
}

Packet& Packet::operator>>(std::string& data)
{
    unsigned short len;
    *this >> len;
    std::copy(m_buffer.begin(), m_buffer.begin() + len, data.begin());
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + len);
    return *this;
}