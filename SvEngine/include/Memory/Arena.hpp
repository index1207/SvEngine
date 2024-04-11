#pragma once

class Arena
{
public:
	static constexpr size_t alignment = alignof(std::max_align_t);
public:
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
public:
	virtual void Reset() = 0;
	virtual size_t Used() = 0;
	virtual byte* Allocate(size_t size) = 0;
	virtual void Deallocate(byte* ptr, size_t size) = 0;
protected:
	size_t AlignUp(size_t n) noexcept
	{
		return (n + alignment - 1) & ~(alignment - 1);
	}
};

template<size_t N>
class FixedArena : public Arena
{
public:
	FixedArena() : m_ptr(m_buffer) { }
	FixedArena(const FixedArena&) = delete;
	FixedArena& operator=(const FixedArena&) = delete;
public:
	static constexpr size_t Size() noexcept { return N; }
public:
	virtual void Reset() noexcept override
	{
		m_ptr = m_buffer;
	}
	virtual size_t Used() noexcept override
	{
		return static_cast<size_t>(m_ptr - m_buffer);
	}
public:
	virtual byte* Allocate(size_t size) override
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
	virtual void Deallocate(byte* ptr, size_t size) noexcept override
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
private:
	alignas(alignment) byte m_buffer[N] = { 0, };
	byte* m_ptr;
};

class DynamicArena : public Arena
{
public:
	DynamicArena(size_t reserve = 0);
	DynamicArena(const DynamicArena&) = delete;
	~DynamicArena();
	DynamicArena& operator=(const DynamicArena&) = delete;
public:
	virtual void Reset() noexcept override;
	virtual size_t Used() noexcept override;
public:
	void Reserve();

	byte* Allocate(size_t size);
	void Deallocate(byte* ptr, size_t size) noexcept;
private:
	alignas(alignment) byte* m_buffer;
	size_t m_size;
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


#define CREATE_FIXED_ARENA(className, arenaSize)\
		static FixedArena<sizeof(className)*arenaSize> className##Arena;\
		IMPLE_ARENA_ALLOC(className)

#define CREATE_DYNAMIC_ARENA(className, reserveSize)\
		static DynamicArena className##Arena(sizeof(className)*reserveSize);\
		IMPLE_ARENA_ALLOC(className)