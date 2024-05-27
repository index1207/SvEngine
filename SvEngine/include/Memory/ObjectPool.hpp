#pragma once

#include "STLAllocator.hpp"

template<class T>
class PoolAllocator : public STLAllocator<T>
{
public:
	T* allocate(size_t count) override;
	void deallocate(T* ptr, size_t size) override;
};

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
		return static_cast<T*>(malloc(sizeof(T)));
	}
	static Vector<T*> Pop(size_t count)
	{
		Vector<T*> objects;
		for (int i = 0; i < count; ++i)
			objects.push_back(Pop());
		return objects;
	}
	static void Push(T* ptr)
	{
		if (ptr)
		{
			m_pool.push(ptr);
		}
	}
	static void Push(T* ptr, size_t count)
	{
		T* p = ptr;
		for (int i = 0; i < count; ++i)
		{
			Push(p);
			p += sizeof(T);
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
void* operator new[](size_t count)\
{\
	return reinterpret_cast<void*>(ObjectPool<className>::Pop(count/sizeof(className)).data());\
}\
void operator delete(void* ptr)\
{\
	ObjectPool<className>::Push(reinterpret_cast<className*>(ptr));\
}\
void operator delete[](void* ptr, size_t count)\
{\
	ObjectPool<className>::Push(reinterpret_cast<className*>(ptr), count/sizeof(className));\
}\
private:\


template<class T>
inline T* PoolAllocator<T>::allocate(size_t count)
{
	return ObjectPool<T>::Pop(count / sizeof(T)).data();
}

template<class T>
inline void PoolAllocator<T>::deallocate(T* ptr, size_t size)
{
	ObjectPool<T>::Push(ptr);
}

template<class T>
ConcurrencyQueue<T*> ObjectPool<T>::m_pool;