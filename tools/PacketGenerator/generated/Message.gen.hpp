#pragma once
#pragma warning(push)
#pragma warning(disable: 26495)
#include <generated/Packet.gen.hpp>

#include <core/Packet.hpp>
#include <util/Types.hpp>

#include <vector>

/* Additional generated packet headers. */


namespace gen {
    class Message
            : public sv::Packet {
    public:
        Message() : sv::Packet(static_cast<unsigned short>(PacketId::MESSAGE)) {
        }
        ~Message() {
    
        }
    public:
        void read() override
        {
            Packet::read();
            *this >> data;
        }
        void write() override
        {
            *this << data;
            finish();
        }
    public:
        std::string data;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, Message& message) {
        pk >> message.data;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const Message& message) {
        pk << message.data;
        return pk;
    }

	class SendMessageReq
            : public sv::Packet {
    public:
        SendMessageReq() : sv::Packet(static_cast<unsigned short>(PacketId::SEND_MESSAGE_REQ)) {
        }
        ~SendMessageReq() {
    
        }
    public:
        void read() override
        {
            Packet::read();
            *this >> msg;
        }
        void write() override
        {
            *this << msg;
            finish();
        }
    public:
        std::vector<Message> msg;
	
    };
    
    inline sv::Packet& operator>>(sv::Packet& pk, SendMessageReq& sendMessageReq) {
        pk >> sendMessageReq.msg;
        return pk;
    }

    inline sv::Packet& operator<<(sv::Packet& pk, const SendMessageReq& sendMessageReq) {
        pk << sendMessageReq.msg;
        return pk;
    }

}
#pragma warning(pop)
