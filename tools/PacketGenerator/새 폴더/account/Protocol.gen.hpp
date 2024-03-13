#pragma once
#pragma warning(push)
#pragma warning(disable: 26495)
#pragma warning(disable: 4100)
#include "Packet.gen.hpp"

#ifdef __UNREAL__
#include "Network/Packet.h"
#elif __SERVER__
#include "Network/Packet.hpp"
#include "Util/Types.hpp"
#endif

#include <vector>

/* Additional headers. */
#include "Struct.gen.hpp"
namespace gen {
namespace account {
    class LoginReq
            : public Packet {
    public:
        LoginReq() : Packet(static_cast<unsigned short>(PacketId::LOGIN_REQ)) {
        }
        ~LoginReq() {
    
        }
    protected:
        virtual void Read() override
        {
            Packet::Read();
            *this >> nickname >> password;
        }
        virtual void Write() override
        {
            *this << nickname << password;
            Finish();
        }
    public:
        String nickname;
		String password;
	
    };
    
    inline Packet& operator>>(Packet& pk, LoginReq& loginReq) {
        pk >> loginReq.nickname >> loginReq.password;
        return pk;
    }

    inline Packet& operator<<(Packet& pk, const LoginReq& loginReq) {
        pk << loginReq.nickname << loginReq.password;
        return pk;
    }

	class RegisterReq
            : public Packet {
    public:
        RegisterReq() : Packet(static_cast<unsigned short>(PacketId::REGISTER_REQ)) {
        }
        ~RegisterReq() {
    
        }
    protected:
        virtual void Read() override
        {
            Packet::Read();
            *this >> nickname >> password;
        }
        virtual void Write() override
        {
            *this << nickname << password;
            Finish();
        }
    public:
        String nickname;
		String password;
	
    };
    
    inline Packet& operator>>(Packet& pk, RegisterReq& registerReq) {
        pk >> registerReq.nickname >> registerReq.password;
        return pk;
    }

    inline Packet& operator<<(Packet& pk, const RegisterReq& registerReq) {
        pk << registerReq.nickname << registerReq.password;
        return pk;
    }

	class LoginRes
            : public Packet {
    public:
        LoginRes() : Packet(static_cast<unsigned short>(PacketId::LOGIN_RES)) {
        }
        ~LoginRes() {
    
        }
    protected:
        virtual void Read() override
        {
            Packet::Read();
            *this >> uuid >> success;
        }
        virtual void Write() override
        {
            *this << uuid << success;
            Finish();
        }
    public:
        String uuid;
		bool success;
	
    };
    
    inline Packet& operator>>(Packet& pk, LoginRes& loginRes) {
        pk >> loginRes.uuid >> loginRes.success;
        return pk;
    }

    inline Packet& operator<<(Packet& pk, const LoginRes& loginRes) {
        pk << loginRes.uuid << loginRes.success;
        return pk;
    }

	class RegisterRes
            : public Packet {
    public:
        RegisterRes() : Packet(static_cast<unsigned short>(PacketId::REGISTER_RES)) {
        }
        ~RegisterRes() {
    
        }
    protected:
        virtual void Read() override
        {
            Packet::Read();
            *this >> success;
        }
        virtual void Write() override
        {
            *this << success;
            Finish();
        }
    public:
        bool success;
	
    };
    
    inline Packet& operator>>(Packet& pk, RegisterRes& registerRes) {
        pk >> registerRes.success;
        return pk;
    }

    inline Packet& operator<<(Packet& pk, const RegisterRes& registerRes) {
        pk << registerRes.success;
        return pk;
    }

}
}
#pragma warning(pop)
