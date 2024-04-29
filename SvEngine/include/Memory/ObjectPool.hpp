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
		return new T();
	}
	static void Push(T* ptr)
	{
		if (ptr)
		{
			ptr->~T();
			m_pool.push(ptr);
		}
	}
private:
	static ConcurrencyQueue<T*> m_pool;
};

template<class T>
ConcurrencyQueue<T*> ObjectPool<T>::m_pool;

