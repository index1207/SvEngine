#include "pch.h"
#include "Thread/ThreadManager.hpp"
#include "Thread/JobSerializer.hpp"

ThreadManager::ThreadManager()
{
	Initialize();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(CallbackType callback)
{
	m_threads.push_back(std::thread([=]()
	{
		Initialize();
		callback();
		Finalize();
	}));
}

void ThreadManager::Join()
{
	for (auto& t : m_threads)
		if (t.joinable()) t.join();

	m_threads.clear();
}

void ThreadManager::Initialize()
{
	static std::atomic<uint16> s_threadId = 1;
	LThreadId = s_threadId.fetch_add(1);
}

void ThreadManager::Finalize()
{
}

void ThreadManager::Execute()
{
	while (true)
	{
		auto now = GetTickCount64();
		if (now > LEndTickCount)
			break;

		auto jobSerializer = GGlobalQueue->Pop();
		if (!jobSerializer)
			break;

		jobSerializer->Flush();
	}
}
