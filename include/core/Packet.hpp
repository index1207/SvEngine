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
        virtual ~Packet() {};
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
        Packet& operator<<(std::string_view data);
        Packet& operator<<(bool data);

        template<class T>
        Packet& operator<<(std::vector<T> data) {
            *this << static_cast<unsigned short>(data.size());
            for(const T& elem : data)
                *this << elem;
            return *this;
        }
    public:
        template<class T>
        Packet& operator>>(T& data) {
            data = 0;
            for(int i = 0; i < sizeof(T); ++i) {
                data += (m_buffer[i] & 0xFF) << (8*(sizeof(T)-i-1));
            }
            m_buffer.erase(m_buffer.begin(), m_buffer.begin()+sizeof(T));
            return *this;
        }
        template<>
        Packet& operator>>(std::string& data) {
            unsigned short len;
            *this >> len;
            data = std::string(m_buffer.begin(), m_buffer.begin()+len);
            m_buffer.erase(m_buffer.begin(), m_buffer.begin()+len);
            return *this;
        }
        template<class _Ty>
        Packet& operator>>(std::vector<_Ty>& data) {
            unsigned short len;
            *this >> len;
            for(unsigned short i = 0; i < len; ++i) {
                _Ty t;
                *this >> t;
                data.push_back(t);
            }
            return *this;
        }
    public:
        void virtual write() = 0;
        void virtual read() = 0;
    public:
        void finish();
        std::vector<char>& data();
    private:
        std::vector<char> m_buffer;
    };
}