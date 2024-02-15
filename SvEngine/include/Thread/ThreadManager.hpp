#pragma once

class ThreadManager
{
	using CallbackType = std::function<void()>;
public:
	ThreadManager();
	~ThreadManager();
public:
	void Launch(CallbackType callback);
	void Join();
public:
	static void Initialize();
	static void Finalize();
	static void Execute();
private:
	ConcurrencyVector<std::thread> m_threads;
};

