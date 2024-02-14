//
// Created by han93 on 2023-12-20.
//
#include "pch.h"
#include "core/Engine.hpp"
#include "util/Console.hpp"

#include "core/ThreadManager.hpp"
#include "core/JobSerializer.hpp"

using namespace sv;

sv::Engine::Engine()
{
	Console::Initialize();

	net::Option::Autorun = false;
	net::Option::Timeout = 10;

	GThreadManager = new ThreadManager;
	GGlobalQueue = new GlobalQueue;
}

sv::Engine::~Engine()
{
	delete GGlobalQueue;
	delete GThreadManager;
}
