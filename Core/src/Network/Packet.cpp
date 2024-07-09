//
// Created by han93 on 2023-12-16.
//
#include "pch.h"

#include "Network/Packet.hpp"

Packet::Packet(uint16 id, PacketType type, int reserve) : m_buffer(2, 0), m_id(0) {
    if (type == RPC)
        id |= 0x8000;
    m_buffer.reserve(reserve);
    memcpy(m_buffer.data(), &id, sizeof(uint16));
}

Vector<char>& Packet::Data() {
    return m_buffer;
}

bool Packet::IsRpcId(uint16 id)
{
    return (0x8000 & id) != 0;
}

void Packet::Parse(std::span<char> buffer) {
    m_buffer = Vector<char>(buffer.begin(), buffer.end());
    Read();
}

uint16 Packet::GetPacketId(std::span<char> buffer)
{
    return *reinterpret_cast<uint16*>(buffer.data());
}

void Packet::Read() {
    *this >> m_id;
}

Packet& Packet::operator<<(unsigned char Data) {
    m_buffer.push_back(static_cast<char>(Data));
    return *this;
}

Packet& Packet::operator<<(unsigned short Data) {
    char buffer[sizeof(Data)] = "";
    memcpy(buffer, &Data, sizeof(Data));
    m_buffer.insert(m_buffer.end(), buffer, buffer + sizeof(Data));
    return *this;
}

Packet& Packet::operator<<(unsigned int Data) {
    char buffer[sizeof(Data)] = "";
    memcpy(buffer, &Data, sizeof(Data));
    m_buffer.insert(m_buffer.end(), buffer, buffer + sizeof(Data));
    return *this;
}

Packet& Packet::operator<<(unsigned long Data) {
    char buffer[sizeof(Data)] = "";
    memcpy(buffer, &Data, sizeof(Data));
    m_buffer.insert(m_buffer.end(), buffer, buffer + sizeof(Data));
    return *this;
}

Packet& Packet::operator<<(unsigned long long Data) {
    char buffer[sizeof(Data)] = "";
    memcpy(buffer, &Data, sizeof(Data));
    m_buffer.insert(m_buffer.end(), buffer, buffer + sizeof(Data));
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
    char buffer[sizeof(Data)] = "";
    memcpy(buffer, &Data, sizeof(Data));
    m_buffer.insert(m_buffer.end(), buffer, buffer + sizeof(Data));
    return *this;
}

Packet& Packet::operator<<(int Data) {
    char buffer[sizeof(Data)] = "";
    memcpy(buffer, &Data, sizeof(Data));
    m_buffer.insert(m_buffer.end(), buffer, buffer + sizeof(Data));
    return *this;
}

Packet& Packet::operator<<(long Data) {
    char buffer[sizeof(Data)] = "";
    memcpy(buffer, &Data, sizeof(Data));
    m_buffer.insert(m_buffer.end(), buffer, buffer + sizeof(Data));
    return *this;
}

Packet& Packet::operator<<(long long Data) {
    char buffer[sizeof(Data)] = "";
    memcpy(buffer, &Data, sizeof(Data));
    m_buffer.insert(m_buffer.end(), buffer, buffer + sizeof(Data));
    return *this;
}

Packet& Packet::operator<<(float Data) {
    char buffer[sizeof(Data)] = "";
    memcpy(buffer, &Data, sizeof(Data));
    m_buffer.insert(m_buffer.end(), buffer, buffer + sizeof(Data));
    return *this;
}

Packet& Packet::operator<<(double Data) {
    char buffer[sizeof(Data)] = "";
    memcpy(buffer, &Data, sizeof(Data));
    m_buffer.insert(m_buffer.end(), buffer, buffer + sizeof(Data));
    return *this;
}

Packet& Packet::operator<<(StringView Data) {
    const auto str = reinterpret_cast<const char*>(Data.data());
    *this << static_cast<int16>(Data.size());
    m_buffer.insert(m_buffer.end(), str, str+Data.size()*2);
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
    return *this;
}

Packet& Packet::operator>>(unsigned int& Data)
{
    std::memcpy(&Data, m_buffer.data(), sizeof(Data));
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + sizeof(Data));
    return *this;
}

Packet& Packet::operator>>(unsigned long& Data)
{
    std::memcpy(&Data, m_buffer.data(), sizeof(Data));
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + sizeof(Data));
    return *this;
}

Packet& Packet::operator>>(unsigned long long& Data)
{
    std::memcpy(&Data, m_buffer.data(), sizeof(Data));
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + sizeof(Data));
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

Packet& Packet::operator>>(String& Data)
{
    unsigned short len;
    *this >> len;
    Data.assign(reinterpret_cast<const wchar_t*>(m_buffer.data()), len);
    m_buffer.erase(m_buffer.begin(), m_buffer.begin() + len*2);
    return *this;
}