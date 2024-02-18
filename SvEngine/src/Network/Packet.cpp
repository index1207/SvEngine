//
// Created by han93 on 2023-12-16.
//
#include "pch.h"

#include "Network/Packet.hpp"

Packet::Packet(unsigned short id, int reserve) : m_buffer(4, 0), m_id(0), m_size(0) {
    m_buffer.reserve(reserve);
    for (int i = sizeof(unsigned short) - 1; i >= 0; --i)
        m_buffer[sizeof(unsigned short) - i - 1] = id >> 8 * i & 0xFF;
}

std::vector<char>& Packet::Data()
{
    return m_buffer;
}

void Packet::Finish()
{
    auto size = static_cast<unsigned short>(m_buffer.size() - 4);
    for (int i = sizeof(size) - 1; i >= 0; --i)
        m_buffer[sizeof(unsigned short) - i + 1] = size >> 8 * i & 0xFF;
}

void Packet::Parse(std::span<char> buffer)
{
    m_buffer = std::vector(buffer.begin(), buffer.end());
    Read();
}

void Packet::Read()
{
    *this >> m_id >> m_size;
}

Packet& Packet::operator<<(unsigned char Data) {
    m_buffer.push_back(static_cast<char>(Data));
    return *this;
}

Packet& Packet::operator<<(unsigned short Data) {
    for(int i = sizeof(Data)-1; i >= 0; --i)
        m_buffer.push_back((Data>>8*i)&0xFF);
    return *this;
}

Packet& Packet::operator<<(unsigned int Data) {
    for(int i = sizeof(Data)-1; i >= 0; --i)
        m_buffer.push_back((Data>>8*i)&0xFF);
    return *this;
}

Packet& Packet::operator<<(unsigned long Data) {
    for(int i = sizeof(Data)-1; i >= 0; --i)
        m_buffer.push_back((Data>>8*i)&0xFF);
    return *this;
}

Packet& Packet::operator<<(unsigned long long Data) {
    for(int i = sizeof(Data)-1; i >= 0; --i)
        m_buffer.push_back((Data >>8*i)&0xFF);
    return *this;
}

Packet& Packet::operator<<(bool Data) {
    return *this << static_cast<unsigned char>(Data);
}

Packet& Packet::operator<<(char Data) {
    m_buffer.push_back(Data);
    return *this;
}

Packet& Packet::operator<<(short Data) {
    for(int i = sizeof(Data)-1; i >= 0; --i)
        m_buffer.push_back((Data>>8*i)&0xFF);
    return *this;
}

Packet& Packet::operator<<(int Data) {
    for(int i = sizeof(Data)-1; i >= 0; --i)
        m_buffer.push_back((Data>>8*i)&0xFF);
    return *this;
}

Packet& Packet::operator<<(long Data) {
    for(int i = sizeof(Data)-1; i >= 0; --i)
        m_buffer.push_back((Data>>8*i)&0xFF);
    return *this;
}

Packet& Packet::operator<<(long long Data) {
    for (int i = sizeof(Data) - 1; i >= 0; --i)
        m_buffer.push_back((Data>>8*i)&0xFF);
    return *this;
}

Packet& Packet::operator<<(float Data) {
    char buffer[sizeof(float)] = "";
    std::memcpy(buffer, &Data, sizeof(float));
    m_buffer.insert(m_buffer.end(), buffer, buffer+sizeof(float));
    return *this;
}

Packet& Packet::operator<<(double Data) {
    char buffer[sizeof(double)] = "";
    std::memcpy(buffer, &Data, sizeof(double));
    m_buffer.insert(m_buffer.end(), buffer, buffer+sizeof(double));
    return *this;
}

Packet& Packet::operator<<(std::wstring_view Data) {
    *this << static_cast<unsigned short>(Data.length()+1)*2;
    m_buffer.insert(m_buffer.end(), Data.begin(), Data.end());
    return *this;
}

Packet& Packet::operator>>(unsigned char& Data)
{
    Data = m_buffer.front();
    m_buffer.erase(m_buffer.begin());
    return *this;
}

Packet& Packet::operator>>(unsigned short& Data)
{
    std::memcpy(&Data, m_buffer.data(), sizeof(Data));
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + sizeof(Data));

    Data = ntohs(Data);
    return *this;
}

Packet& Packet::operator>>(unsigned int& Data)
{
    std::memcpy(&Data, m_buffer.data(), sizeof(Data));
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + sizeof(Data));
    Data = static_cast<unsigned int>(ntohl(static_cast<u_long>(Data)));
    Data = ntohl(Data);
    return *this;
}

Packet& Packet::operator>>(unsigned long& Data)
{
    std::memcpy(&Data, m_buffer.data(), sizeof(Data));
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + sizeof(Data));
    Data = ntohl(Data);
    return *this;
}

Packet& Packet::operator>>(unsigned long long& Data)
{
    std::memcpy(&Data, m_buffer.data(), sizeof(Data));
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + sizeof(Data));
    Data = ntohll(Data);
    return *this;
}

Packet& Packet::operator>>(bool& Data)
{
    unsigned char t;
    *this >> t;
    Data = static_cast<bool>(t);

    return *this;
}

Packet& Packet::operator>>(char& Data)
{
    unsigned char t;
    *this >> t;
    Data = static_cast<char>(t);

    return *this;
}

Packet& Packet::operator>>(short& Data)
{
    unsigned short t;
    *this >> t;
    Data = static_cast<short>(t);
    return *this;
}

Packet& Packet::operator>>(long& Data)
{
    unsigned long t;
    *this >> t;
    Data = static_cast<long>(t);
    return *this;
}

Packet& Packet::operator>>(long long& Data)
{
    unsigned long long t;
    *this >> t;
    Data = static_cast<long long>(t);
    return *this;
}

Packet& Packet::operator>>(int& Data)
{
    unsigned int t = 0;
    *this >> t;
    Data = static_cast<int>(t);
    return *this;
}

Packet& Packet::operator>>(float& Data)
{
    std::memcpy(&Data, m_buffer.data(), sizeof(Data));
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + sizeof(Data));
    return *this;
}

Packet& Packet::operator>>(double& Data)
{
    std::memcpy(&Data, m_buffer.data(), sizeof(Data));
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + sizeof(Data));
    return *this;
}

Packet& Packet::operator>>(std::wstring& Data)
{
    unsigned short len;
    *this >> len;
    std::copy(m_buffer.begin(), m_buffer.begin() + len, Data.begin());
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + len);
    return *this;
}