#pragma once

#include <thread>

class ThreadManager
{
	using CallbackType = std::function<void()>;
public:
	ThreadManager();
	~ThreadManager();
public:
	void Launch(CallbackType callback);
	void Join();
	void Terminate();
public:
	static void Initialize();
	static void Finalize();
private:
	std::mutex m_lock;
	Vector<std::thread> m_threads;
};

