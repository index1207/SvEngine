//
// Created by han93 on 2023-12-20.
//
#include "pch.h"
#include "Subsystem/Engine.hpp"
#include "Functor.hpp"

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

void Engine::EnqueueDbFunctor(const std::shared_ptr<Functor>& functor)
{
	m_dbFunctorQue.push(functor);
}

void Engine::Initialize()
{
}

void Engine::ExecuteIo(int32 count)
{
	for (int32 i = 0; i < count; ++i)
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
	new std::thread([this] {
		while (true)
		{
			if (m_dbFunctorQue.empty())
				std::this_thread::sleep_for(std::chrono::milliseconds(EngineOption::WaitTime));
			else
			{
				while (!m_dbFunctorQue.empty())
				{
					std::shared_ptr<Functor> functor;
					if (m_dbFunctorQue.try_pop(functor) && functor)
					{
						(*functor)();
						functor = nullptr;
					}
				}
			}
		}
	});
}

void Engine::Fetch()
{
	while (true)
	{
		if (m_functorQue.empty())
			std::this_thread::sleep_for(std::chrono::milliseconds(EngineOption::WaitTime));
		else
		{
			while (!m_functorQue.empty())
			{
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
	}
}
