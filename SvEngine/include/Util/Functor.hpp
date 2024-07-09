#pragma once

class Functor
{
	USE_POOL(Functor)
public:
	Functor() = default;
	DLLEXPORT ~Functor() noexcept;

	template<class T, class... Args>
	DLLEXPORT Functor(uint64 reserve, T* ptr, void(T::*method)(Args...), Args... args)
	{
		m_executeTime = GetTickCount64() + reserve;
		m_functor = std::bind(method, ptr, args...);
	}

	template<class T, class... Args>
	DLLEXPORT Functor(T* ptr, void(T::*method)(Args...), Args... args)
		: Functor(0, ptr, method, args...)
	{
	}

	template<class... Args>
	DLLEXPORT Functor(uint64 reserve, std::function<void(Args...)> func, Args... args)
	{
		m_executeTime = GetTickCount64() + reserve;
		m_functor = std::bind(func, args...);
	}

	template<class... Args>
	DLLEXPORT Functor(std::function<void(Args...)> func, Args... args)
		: Functor(0, func, args...)
	{
	}
public:
	DLLEXPORT inline void operator()() const { m_functor(); }
	DLLEXPORT bool operator<(const Functor& functor) const { return m_executeTime > functor.m_executeTime; }
	DLLEXPORT inline uint64 GetExecuteTime() const { return m_executeTime; }
private:
	std::function<void()> m_functor;
	uint64 m_executeTime;
};

template<class T>
struct Executable
{
	template<class... Args>
	DLLEXPORT void Enqueue(void(T::*method)(Args...), Args... args)
	{
		auto functor = MakeShared<Functor>(static_cast<T*>(this), method, args...);
		GEngine->EnqueueFunctor(functor);
	}

	template<class... Args>
	DLLEXPORT void Enqueue(uint64 reserve, void(T::* method)(Args...), Args... args)
	{
		auto functor = MakeShared<Functor>(reserve, static_cast<T*>(this), method, args...);
		GEngine->EnqueueFunctor(functor);
	}

	template<class... Args>
	DLLEXPORT void Enqueue(std::function<void()> func, Args... args)
	{
		auto functor = MakeShared<Functor>(func, args...);
		GEngine->EnqueueFunctor(functor);
	}

	template<class... Args>
	DLLEXPORT void Enqueue(uint64 reserve, std::function<void()> func, Args... args)
	{
		auto functor = MakeShared<Functor>(reserve, func, args...);
		GEngine->EnqueueFunctor(functor);
	}
};