#include "pch.h"
#include "Functor.hpp"

Functor::~Functor() noexcept
{
}

Functor::Functor(uint64 reserve, std::function<void()> func)
{
	m_executeTime = GetTickCount64() + reserve;
	m_functor = func;
}

Functor::Functor(std::function<void()> func)
	: Functor(0, func)
{
}
