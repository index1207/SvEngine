#include "pch.h"
#include "Memory/Arena.hpp"

DynamicArena::DynamicArena(size_t reserve)
{
	m_size = reserve > 0 ? reserve : 1;
	Reserve();
}

DynamicArena::~DynamicArena()
{
	Reset();
}

void DynamicArena::Reset() noexcept
{
	if (m_buffer)
		free(m_buffer);
}

size_t DynamicArena::Used() noexcept
{
	return m_ptr - m_buffer;
}

void DynamicArena::Reserve()
{
	auto newBuffer = static_cast<byte*>(malloc(m_size));
	auto offset = Used();

	__analysis_assume(newBuffer != nullptr);

	if (m_buffer != nullptr)
	{
		memcpy(newBuffer, m_buffer, m_size);
	}
	m_buffer = newBuffer;
	m_ptr = m_buffer + offset;
}

byte* DynamicArena::Allocate(size_t size)
{
	const auto alignNum = AlignUp(size);
	const auto availableBytes = static_cast<decltype(alignNum)>(m_buffer + m_size - m_ptr);
	if (availableBytes < alignNum)
	{
		m_size = alignNum * 2;
		Reserve();
	}
	auto* ptr = m_ptr;
	m_ptr += size;
	return ptr;
}

void DynamicArena::Deallocate(byte* ptr, size_t size) noexcept
{
	size = AlignUp(size);
	if (ptr + size == m_ptr) {
		m_ptr = ptr;
	}
}