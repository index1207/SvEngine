#include "pch.h"
#include "core/ThreadManager.hpp"
#include "core/JobSerializer.hpp"

ThreadManager::ThreadManager()
{
	initialize();
}

ThreadManager::~ThreadManager()
{
	join();
}

void ThreadManager::launch(CallbackType callback)
{
	m_threads.push_back(std::thread([=]()
	{
		initialize();
		callback();
		finalize();
	}));
}

void ThreadManager::join()
{
	for (auto& t : m_threads)
		if (t.joinable()) t.join();

	m_threads.clear();
}

void ThreadManager::initialize()
{
	static std::atomic<uint16> s_threadId = 1;
	LThreadId = s_threadId.fetch_add(1);
}

void ThreadManager::finalize()
{
}

void ThreadManager::executeGlobalJobSerializer()
{
	while (true)
	{
		auto now = GetTickCount64();
		if (now > LEndTickCount)
			break;

		auto jobSerializer = GGlobalQueue->Pop();
		if (!jobSerializer)
			break;

		jobSerializer->flush();
	}
}
