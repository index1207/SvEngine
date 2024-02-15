#pragma once

class ThreadManager
{
	using CallbackType = std::function<void()>;
public:
	ThreadManager();
	~ThreadManager();
public:
	void launch(CallbackType callback);
	void join();
public:
	static void initialize();
	static void finalize();
	static void executeGlobalJobSerializer();
private:
	ConcurrencyVector<std::thread> m_threads;
};

