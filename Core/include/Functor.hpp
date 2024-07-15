#pragma once

#include "Subsystem/Engine.hpp"

using CallbackType = std::function<void()>;

class SVENGINE_API Functor
{
	USE_POOL(Functor)
public:
	Functor() = default;
	~Functor() noexcept;

	Functor(CallbackType functor);

	template<class T, class... Args>
	Functor(void(T::*method)(Args...), T* ptr, Args... args)
	{
		functor = std::bind(method, ptr, args...);
	}
public:
	inline void operator()() const { functor(); }
private:
	CallbackType functor;
};	

class SVENGINE_API DelayedFunctor
{
public:
	DelayedFunctor() = default;
	DelayedFunctor(uint64 execTime, std::shared_ptr<Functor> functor);
public:
	bool operator<(const DelayedFunctor& other) const;
	bool operator>(const DelayedFunctor& other) const;
public:
	uint64 execTime;
	std::shared_ptr<Functor> functor;
};

class SVENGINE_API FunctorProcessor
{
public:
	FunctorProcessor() = default;
public:
	void Push(std::shared_ptr<Functor> functor);
	void Push(uint64 delay, std::shared_ptr<Functor> functor);
public:
	void Flush();
	void Fetch();
private:
	ConcurrencyQueue<std::shared_ptr<Functor>> m_functorQue;
	ConcurrencyPriorityQueue<DelayedFunctor, std::greater<DelayedFunctor>> m_delayedFuncQue;
};