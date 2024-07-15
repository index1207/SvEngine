#include "pch.h"
#include "Subsystem/Engine.hpp"

Engine::Engine()
	: m_funcProc(std::make_unique<FunctorProcessor>())
{
	Console::Initialize();

	net::Option::Autorun = false;
}

Engine::~Engine()
{
}

void Engine::Run(int32 io)
{
	ProcessIo(io);
	auto logic = new std::thread(&Engine::ProcessLogic, this);
	logic->join();
}

void Engine::Initialize()
{
}

void Engine::ProcessIo(int32 count)
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
}

void Engine::ProcessLogic()
{
	while (true)
	{
		m_funcProc->Fetch();
		m_funcProc->Flush();
		std::this_thread::sleep_for(std::chrono::milliseconds(WorkTick));
	}
}
