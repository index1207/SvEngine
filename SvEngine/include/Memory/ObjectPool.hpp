#pragma once
template<class T>
class ObjectPool
{
public:
	T* Pop()
	{
		if (!m_pool.empty())
		{
			T* ptr;
			while (!m_pool.try_pop(ptr));
			return ptr;
		}
		return new T();
	}
	void Push(T* ptr)
	{
		ptr->~T();
		m_pool.push(ptr);
	}
private:
	ConcurrencyQueue<T*> m_pool;
};

