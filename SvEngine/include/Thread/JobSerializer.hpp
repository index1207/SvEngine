#pragma once

class Job
{
	using CallbackType = std::function<void()>;
public:
	inline Job(CallbackType&& callback) : m_callback(std::move(callback)) { }
	
	template<class T, class _Ret, class ...Args>
	inline Job(std::shared_ptr<T> owner, _Ret(T::*method)(Args...), Args&&... args)
	{
		m_callback = [owner, method, args...]() { (owner.get()->*method(args...)); };
	}
	__forceinline void operator()() { m_callback(); }
private:
	CallbackType m_callback;
};

class JobSerializer : public std::enable_shared_from_this<JobSerializer>
{
	using CallbackType = std::function<void()>;
public:
	inline void Launch(CallbackType&& callback);
	
	template<class T, class _Ret, class ...Args>
	inline void Launch(_Ret(T::*method)(Args...), Args&&... args)
	{
		auto owner = std::static_pointer_cast<T>(shared_from_this());
		Push(std::make_shared<Job>(owner, method, std::forward<Args>(args)...));
	}

	inline void Push(std::shared_ptr<Job> jobPtr);

	void Flush();
private:
	ConcurrencyQueue<std::shared_ptr<Job>> m_jobQue;
	std::atomic<uint32> m_jobCount;
};

class JobQueue
{
public:
	JobQueue();
	~JobQueue();

	void Push(std::shared_ptr<JobSerializer> jobSerializer);
	std::shared_ptr<JobSerializer> Pop();
private:
	ConcurrencyQueue<std::shared_ptr<JobSerializer>> m_jobSerializerQue;
};