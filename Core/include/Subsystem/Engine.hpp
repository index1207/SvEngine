#pragma once

#include "Functor.hpp"

class SVENGINE_API Engine {
    enum EngineOption
    {
        WorkTick = 10,
    };
public:
    Engine();
    ~Engine();
public:
    void Initialize();

    void Run(int32 io);


	template<class T, class... Args>
	void Launch(void(T::* method)(Args...), T* ptr, Args... args)
	{
		auto functor = MakeShared<Functor>(method, ptr, args...);
		m_funcProc->Push(functor);
	}

	template<class T, class... Args>
	void Launch(uint64 delay, void(T::* method)(Args...), T* ptr, Args... args)
	{
		auto functor = MakeShared<Functor>(method, ptr, args...);
		m_funcProc->Push(delay, functor);
	}
private:
    void ProcessIo(int32 count);
    void ProcessLogic();
private:
    std::unique_ptr<FunctorProcessor> m_funcProc;
};

extern Engine* GEngine;

template<class T>
struct Runnable
{
	template<class... Args>
	void Run(void(T::* method)(Args...), Args... args)
	{
		GEngine->Launch(method, static_cast<T*>(this), args...);
	}

	template<class... Args>
	void Run(uint64 delay, void(T::* method)(Args...), Args... args)
	{
		GEngine->Launch(delay, method, static_cast<T*>(this), args...);
	}
};