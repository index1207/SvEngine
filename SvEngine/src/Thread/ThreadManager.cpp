#include "pch.h"
#include "Thread/ThreadManager.hpp"
#include "Thread/JobSerializer.hpp"

ThreadManager::ThreadManager()
{
	Initialize();
}

ThreadManager::~ThreadManager()
{
}

void ThreadManager::Launch(CallbackType callback, CallbackType tlsInit)
{
	m_threads.push_back(new std::thread([=, this] {
		Initialize();
		tlsInit();
		callback();
		Finalize();
	}));
}

void ThreadManager::Join()
{
	for (auto& t : m_threads)
		if (t->joinable()) t->join();
}

void ThreadManager::Terminate()
{
	for (auto& t : m_threads)
		delete t;
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