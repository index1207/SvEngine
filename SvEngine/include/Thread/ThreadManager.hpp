#pragma once

#include <thread>

class ThreadManager
{
	using CallbackType = std::function<void()>;
public:
	ThreadManager();
	~ThreadManager();
public:
	void Launch(CallbackType callback, CallbackType tlsInit);
	void Join();
	void Terminate();
public:
	static void Initialize();
	static void Finalize();
private:
	ConcurrencyVector<std::thread*> m_threads;
};

