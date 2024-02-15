//
// Created by han93 on 2023-12-20.
//
#include "pch.h"
#include "Engine.hpp"

#include "Thread/ThreadManager.hpp"
#include "Thread/JobSerializer.hpp"
#include "Database/DbConnectionPool.hpp"

Engine::Engine()
{
	Console::Initialize();

	net::Option::Autorun = false;
	net::Option::Timeout = 10;

	GThreadManager = new ThreadManager;
	GGlobalQueue = new GlobalQueue;
	GDbConnectionPool = new DbConnectionPool;
}

Engine::~Engine()
{
	delete GGlobalQueue;
	delete GDbConnectionPool;
	delete GThreadManager;
}
