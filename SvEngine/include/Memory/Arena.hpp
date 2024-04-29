#pragma once
	
template<class T, class... Args>
	requires std::is_class_v<T>
static inline std::shared_ptr<T> MakeShared(Args&&... args)
{
	return std::shared_ptr<T>(new T(std::forward<Args>(args)...));
}
template<class T>
	requires std::is_unbounded_array_v<T>
static inline std::shared_ptr<T> MakeShared(const size_t size)
{
	using Ty = action::PeelArrayType<T>::type;
	return std::shared_ptr<T>(new Ty[size]);
}

template<class T, size_t N>
class Arena
{
	static constexpr size_t alignment = alignof(std::max_align_t);
public:
	Arena() : m_ptr(m_buffer) { }
	Arena(const Arena&) = delete;
	Arena& operator=(const Arena&) = delete;
public:
	static constexpr size_t Size() noexcept { return N; }
public:
	virtual void Reset() noexcept
	{
		m_ptr = m_buffer;
	}
	virtual size_t Used() noexcept
	{
		return static_cast<size_t>(m_ptr - m_buffer);
	}
public:
	virtual byte* Allocate(size_t size)
	{
		const auto alignNum = AlignUp(size);
		const auto availableBytes = static_cast<decltype(alignNum)>(m_buffer + N - m_ptr);
		if (availableBytes >= alignNum)
		{
			auto* ptr = m_ptr;
			m_ptr += alignNum;
			return ptr;
		}
		return static_cast<byte*>(::operator new(size));
	}
	virtual void Deallocate(byte* ptr, size_t size) noexcept
	{
		if (IsArenaMemory(ptr))
		{
			size = AlignUp(size);
			if (ptr + size == m_ptr) {
				m_ptr = ptr;
			}
		}
		else ::operator delete(ptr);
	}
private:
	bool IsArenaMemory(const byte* ptr) noexcept
	{
		return ptr >= m_buffer && ptr <= m_buffer + N;
	}
	inline size_t AlignUp(size_t n) noexcept
	{
		return (n + alignment - 1) & ~(alignment - 1);
	}
private:
	alignas(alignment) byte m_buffer[sizeof(T) * N] = { 0, };
	byte* m_ptr;
};

#define USE_ARENA() public:\
void* operator new(size_t size);\
void operator delete(void* ptr);\
void* operator new[](size_t size);\
void operator delete[](void* ptr, size_t size);\
private:\

#define IMPLE_ARENA_ALLOC(className)\
void* className::operator new(size_t size)\
{\
	return className##Arena.Allocate(size);\
}\
void className::operator delete(void* ptr)\
{\
	className##Arena.Deallocate(static_cast<byte*>(ptr), sizeof(className));\
}\
void* className::operator new[](size_t size)\
{\
	return className##Arena.Allocate(size);\
}\
void className::operator delete[](void* ptr, size_t size)\
{\
	className##Arena.Deallocate(static_cast<byte*>(ptr), size);\
}\


#define CREATE_ARENA(className, arenaSize)\
		static Arena<className, arenaSize> className##Arena;\
		IMPLE_ARENA_ALLOC(className)