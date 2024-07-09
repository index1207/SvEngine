//
// Created by han93 on 2023-12-20.
//
#include "pch.h"
#include "Subsystem/Engine.hpp"

#include "Util/Functor.hpp"

Engine::Engine()
{
	Console::Initialize();

	net::Option::Autorun = false;
	net::Option::Timeout = WaitTime;
}

Engine::~Engine()
{
}

void Engine::Run(int32 io)
{
	ExecuteIo(io);
	Fetch();
}

void Engine::EnqueueFunctor(const std::shared_ptr<Functor>& functor)
{
	m_functorQue.push(functor);
}

void Engine::Initialize()
{
}

void Engine::ExecuteIo(int32 threadCount)
{
	for (int i = 0; i < threadCount; ++i)
	{
		new std::thread([] {
			{
				while (true)
				{
					IoSystem::instance().worker(); // IOCP I/O Worker
				}
			}
		});
	}
}

void Engine::Fetch()
{
	while (true)
	{
		if (m_functorQue.empty())
			std::this_thread::sleep_for(std::chrono::milliseconds(EngineOption::WaitTime));

		std::shared_ptr<Functor> functor;
		if (m_functorQue.try_pop(functor) && functor)
		{
			if (functor->GetExecuteTime() <= GetTickCount64())
			{
				(*functor)();
				functor = nullptr;
			}
			else
			{
				m_functorQue.push(functor);
			}
		}
	}
}
