//
// Created by han93 on 2023-12-20.
//
#include "pch.h"
#include "Subsystem/Engine.hpp"

#include "Thread/ThreadManager.hpp"
#include "Thread/JobSerializer.hpp"
#include "Database/DBConnectionPool.hpp"

Engine::Engine()
{
	Console::Initialize();

	net::Option::Autorun = false;
	net::Option::Timeout = EngineOption::GQCSTimeout;
}

Engine::~Engine()
{
	delete m_threadManager;
	delete m_dbConnectionPool;
	delete m_jobTimer;
}

void Engine::ExecuteThread(int32 io, int32 logic, bool mainAsLogic)
{
	ExecuteIo(io);
	ExecuteLogic(logic, mainAsLogic);
	m_threadManager->Join();
}

void Engine::AddJobQueue(JobQueue* jobQue)
{
	m_jobQues.push_back(jobQue);
}

void Engine::PushJob(std::shared_ptr<class Job> job)
{
	uint32 minSize = UINT_MAX;
	JobQueue* jobQue = nullptr;
	for (const auto& que : m_jobQues)
	{
		if (minSize > que->GetSize())
		{
			minSize = que->GetSize();
			jobQue = que;
		}
	}
	if (jobQue)
	{
		jobQue->Push(job);
	}
}

void Engine::Initialize()
{
	m_threadManager = new ThreadManager;
	m_dbConnectionPool = new DBConnectionPool;
	m_jobTimer = new JobTimer;
}

void Engine::ExecuteLogic(int32 threadCount, bool useMainThrd)
{
	if (!useMainThrd)
		--threadCount;

	auto f = [this] {
		while (true)
		{
			m_jobTimer->Distribute(GetTickCount64());
			std::this_thread::sleep_for(std::chrono::milliseconds(EngineOption::FlushTick));
			LJobQueue->Flush();
		}
	};
	for (int i = 0; i < threadCount; ++i)
	{
		m_threadManager->Launch([=]()
		{
			f();
		});
	}

	if (useMainThrd) f();
}

void Engine::ExecuteIo(int32 threadCount)
{
	for (int i = 0; i < threadCount; ++i)
	{
		m_threadManager->Launch([=]()
		{
			while (true)
			{
				IoSystem::instance().worker(); // IOCP I/O Worker
			};
		});
	}
}