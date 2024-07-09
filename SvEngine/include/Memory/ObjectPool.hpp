#pragma once

template<class T>
class ObjectPool
{
public:
	DLLEXPORT static T* Pop()
	{
		if (!m_pool.empty())
		{
			T* ptr;
			while (!m_pool.try_pop(ptr));
			return ptr;
		}
		return static_cast<T*>(malloc(sizeof(T)));
	}
	DLLEXPORT static void Push(T* ptr)
	{
		if (ptr)
		{
			m_pool.push(ptr);
		}
	}
private:
	static ConcurrencyQueue<T*> m_pool;
};

#define USE_POOL(className)\
public:\
	void* operator new(size_t)\
	{\
		return reinterpret_cast<void*>(ObjectPool<className>::Pop());\
	}\
	void operator delete(void* ptr)\
	{\
		ObjectPool<className>::Push(reinterpret_cast<className*>(ptr));\
	}\
private:\

template<class T>
ConcurrencyQueue<T*> ObjectPool<T>::m_pool;