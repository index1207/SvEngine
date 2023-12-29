#pragma once

#include <winsock2.h>
#include <Windows.h>

#include <vector>
#include <thread>
#include <mutex>

namespace net
{
	class IoSystem
	{
        friend class Socket;
	public:
		IoSystem();
		~IoSystem();
    public:
		void push(SOCKET s);
    public:
        void worker();
        void dispatch(class Context* context, DWORD numOfBytes, bool isSuccess);
    private:
		HANDLE _hcp;
        const Socket* _listeningSocket;
        std::vector<std::thread*> _workers;
        std::mutex _mtx;
	};

    static IoSystem ioSystem;
}