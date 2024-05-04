#pragma once
template<class T>
class ObjectPool
{
public:
	static T* Pop()
	{
		if (!m_pool.empty())
		{
			T* ptr;
			while (!m_pool.try_pop(ptr));
			return ptr;
		}
		return reinterpret_cast<T*>(malloc(sizeof(T)));
	}
	static void Push(T* ptr)
	{
		if (ptr)
		{
			m_pool.push(ptr);
		}
	}
private:
	static ConcurrencyQueue<T*> m_pool;
};

template<class T>
ConcurrencyQueue<T*> ObjectPool<T>::m_pool;

#define USE_POOL(className)\
void* operator new(size_t size)\
{\
	return reinterpret_cast<void*>(ObjectPool<className>::Pop());\
}\
void operator delete(void* ptr)\
{\
	ObjectPool<className>::Push(reinterpret_cast<className*>(ptr));\
}\
