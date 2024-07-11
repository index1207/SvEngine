#pragma once

class SVENGINE_API Functor
{
	USE_POOL(Functor)
public:
	Functor() = default;
	~Functor() noexcept;

	template<class T, class... Args>
	Functor(uint64 reserve, T* ptr, void(T::*method)(Args...), Args... args)
	{
		m_executeTime = GetTickCount64() + reserve;
		m_functor = std::bind(method, ptr, args...);
	}

	template<class T, class... Args>
	Functor(T* ptr, void(T::*method)(Args...), Args... args)
		: Functor(0, ptr, method, args...)
	{
	}

	Functor(uint64 reserve, std::function<void()> func);

	Functor(std::function<void()> func);
public:
	inline void operator()() const { m_functor(); }
	bool operator<(const Functor& functor) const { return m_executeTime > functor.m_executeTime; }
	inline uint64 GetExecuteTime() const { return m_executeTime; }
private:
	std::function<void()> m_functor;
	uint64 m_executeTime;
};

template<class T>
struct Runnable
{
	template<class... Args>
	void Run(void(T:: * method)(Args...), Args... args)
	{
		auto functor = MakeShared<Functor>(static_cast<T*>(this), method, args...);
		GEngine->EnqueueFunctor(functor);
	}

	template<class... Args>
	void Run(uint64 reserve, void(T::* method)(Args...), Args... args)
	{
		auto functor = MakeShared<Functor>(reserve, static_cast<T*>(this), method, args...);
		GEngine->EnqueueFunctor(functor);
	}

	template<class... Args>
	void Run(std::function<void()> func, Args... args)
	{
		auto functor = MakeShared<Functor>(func, args...);
		GEngine->EnqueueFunctor(functor);
	}

	template<class... Args>
	void Run(uint64 reserve, std::function<void()> func, Args... args)
	{
		auto functor = MakeShared<Functor>(reserve, func, args...);
		GEngine->EnqueueFunctor(functor);
	}
};