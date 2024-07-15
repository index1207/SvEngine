#include "pch.h"
#include "Functor.hpp"

Functor::~Functor() noexcept
{
}

Functor::Functor(CallbackType functor)
{
	functor = functor;
}

DelayedFunctor::DelayedFunctor(uint64 execTime, std::shared_ptr<Functor> functor)
	: execTime(execTime), functor(functor)
{
}

bool DelayedFunctor::operator<(const DelayedFunctor& other) const
{
	return execTime < other.execTime;
}

bool DelayedFunctor::operator>(const DelayedFunctor& other) const
{
	return execTime > other.execTime;
}

void FunctorProcessor::Push(std::shared_ptr<Functor> functor)
{
	m_functorQue.push(functor);
}

void FunctorProcessor::Push(uint64 delay, std::shared_ptr<Functor> functor)
{
	m_delayedFuncQue.push(DelayedFunctor(GetTickCount64() + delay, functor));
}

void FunctorProcessor::Flush()
{
	while (!m_functorQue.empty())
	{
		std::shared_ptr<Functor> functor;
		if (m_functorQue.try_pop(functor))
			(*functor)();
	}
}

void FunctorProcessor::Fetch()
{
	while (!m_delayedFuncQue.empty())
	{
		DelayedFunctor delayedFunc;
		if (m_delayedFuncQue.try_pop(delayedFunc))
		{
			if (GetTickCount64() < delayedFunc.execTime)
			{
				m_delayedFuncQue.push(delayedFunc);
				break;
			}
			else
			{
				m_functorQue.push(delayedFunc.functor);
			}
		}
	}
}
