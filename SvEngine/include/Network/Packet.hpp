//
// Created by han93 on 2023-12-16.
//

#pragma once

#include "Memory/ObjectPool.hpp"

class Session;

enum PacketType : uint8
{
    Generated,
    RPC
};

class DLLEXPORT Packet {
    friend Session;
    using HandlerFunc = std::function<void(std::shared_ptr<Session>)>;
public:
    Packet() = default;
    Packet(uint16 id, PacketType type = Generated, int reserve = 1024);
    virtual ~Packet() {};
public:
    Packet& operator<<(unsigned char Data);
    Packet& operator<<(unsigned short Data);
    Packet& operator<<(unsigned int Data);
    Packet& operator<<(unsigned long Data);
    Packet& operator<<(unsigned long long Data);
    Packet& operator<<(bool Data);
    Packet& operator<<(char Data);
    Packet& operator<<(short Data);
    Packet& operator<<(int Data);
    Packet& operator<<(long Data);
    Packet& operator<<(long long Data);
    Packet& operator<<(float Data);
    Packet& operator<<(double Data);
    Packet& operator<<(StringView Data);

    template<class T>
    Packet& operator<<(std::vector<T> Data) {
        *this << static_cast<uint16>(Data.size());
        for(const T& elem : Data)
            *this << elem;
        return *this;
    }
public:
    Packet& operator>>(long long& Data);
    Packet& operator>>(unsigned char& Data);
    Packet& operator>>(unsigned short& Data);
    Packet& operator>>(unsigned int& Data);
    Packet& operator>>(unsigned long& Data);
    Packet& operator>>(unsigned long long& Data);
    Packet& operator>>(bool& Data);
    Packet& operator>>(char& Data);
    Packet& operator>>(short& Data);
    Packet& operator>>(int& Data);
    Packet& operator>>(long& Data);
    Packet& operator>>(float& Data);
    Packet& operator>>(double& Data);
    Packet& operator>>(String& Data);

    template<class T>
    Packet& operator>>(std::vector<T>& Data) {
        unsigned short len;
        *this >> len;
        for(unsigned short i = 0; i < len; ++i) {
            T t;
            *this >> t;
            Data.push_back(t);
        }
        return *this;
    }
protected:
    void virtual Write() {};
    void virtual Read();
public:
    void SetId(uint16 id) { m_id = id; }
    uint16 GetId() const { return m_id; }
public:
    void Parse(std::span<char> buffer);

    static uint16 GetPacketId(std::span<char> buffer);

    template<class T>
    DLLEXPORT static std::unique_ptr<T> ParseFrom(std::span<char> buffer)
    {
        auto pk = MakeUnique<T>();
        pk->Parse(buffer);
        pk->SetId(GetPacketId(buffer));
        return std::move(pk);
    }
    Vector<char>& Data();
    
    static bool IsRpcId(uint16 id);
private:
    Vector<char> m_buffer;
    unsigned short m_id;
};
