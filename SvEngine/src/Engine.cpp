//
// Created by han93 on 2023-12-20.
//
#include "pch.h"
#include "Engine.hpp"

#include "Thread/ThreadManager.hpp"
#include "Thread/JobSerializer.hpp"
#include "Database/DBConnectionPool.hpp"

Engine* GEngine = new Engine;

Engine::Engine()
{
	Console::Initialize();

	net::Option::Autorun = false;
	net::Option::Timeout = 10;

	m_threadManager = new ThreadManager;
	m_jobQue = new JobQueue;
	m_dbConnectionPool = new DBConnectionPool;
}

Engine::~Engine()
{
	delete m_threadManager;
	delete m_jobQue;
	delete m_dbConnectionPool;
}

void Engine::ExecuteIocpLogic(int32 threadCount, bool useMainThrd)
{
	for (int i = 0; i < threadCount; ++i)
	{
		m_threadManager->Launch([=]()
		{
			ExecuteWorker();
		});
	}

	if (useMainThrd) ExecuteWorker();

	m_threadManager->Join();
}

void Engine::ExecuteWorker()
{
	while (true)
	{
		LEndTickCount = GetTickCount64() + WORK_TICK; // Wait for WORK_TICK

		IoSystem::instance().worker(); // IOCP I/O

		while (true)
		{
			auto now = GetTickCount64();
			if (now > LEndTickCount)
				break;

			auto jobSerializer = m_jobQue->Pop();
			if (!jobSerializer)
				break;

			jobSerializer->Flush();
		}
	}
}
