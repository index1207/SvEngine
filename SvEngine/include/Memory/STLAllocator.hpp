#pragma once

template<class T>
class STLAllocator
{
public:
	using value_type = T;

	STLAllocator() = default;

	template<class U>
	STLAllocator(const STLAllocator<U>&) { }

	virtual T* allocate(size_t count)
	{
		return reinterpret_cast<T*>(malloc(count));
	};
	virtual void deallocate(T* ptr, size_t count)
	{
		free(ptr);
	};
};

