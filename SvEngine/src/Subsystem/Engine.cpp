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
	net::Option::Timeout = WorkTick;
}

Engine::~Engine()
{
	delete m_threadManager;
	delete m_dbConnectionPool;
	delete m_jobTimer;
}

void Engine::AddSerializer(JobSerializer* serializer)
{
	m_serializerQue.push(serializer);
}

void Engine::ExecuteThread(int32 io, int32 logic)
{
	ExecuteLogic(logic);
	ExecuteIo(io);
}

void Engine::Initialize()
{
	m_threadManager = new ThreadManager;
	m_dbConnectionPool = new DBConnectionPool;
	m_jobTimer = new JobTimer;
}

void Engine::ExecuteLogic(int32 threadCount, std::function<void()> tlsInit)
{
	auto worker = [this] {
		while (true)
		{
			m_jobTimer->Distribute(GetTickCount64());

			if (!m_serializerQue.empty())
			{
				JobSerializer* jobSerializer;
				if (m_serializerQue.try_pop(jobSerializer))
				{
					jobSerializer->Flush();
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(WorkTick));
		}
	};
	for (int i = 0; i < threadCount; ++i)
	{
		m_threadManager->Launch([=]()
		{
			worker();
		}, tlsInit);
	}
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