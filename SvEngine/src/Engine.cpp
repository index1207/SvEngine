//
// Created by han93 on 2023-12-20.
//
#include "pch.h"
#include "Engine.hpp"

Engine* GEngine = new Engine;

Engine::Engine()
{
	Console::Initialize();

	net::Option::Autorun = false;
	net::Option::Timeout = 10;

	m_threadManager = new ThreadManager;
	m_jobQue = new JobQueue;
}

Engine::~Engine()
{
	delete m_threadManager;
	delete m_jobQue;
}

void Engine::ExecuteIocpLogic()
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
