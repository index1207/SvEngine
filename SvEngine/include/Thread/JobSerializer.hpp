#pragma once

class Job
{
	USE_ARENA()
	using CallbackType = std::function<void()>;
public:
	Job(CallbackType&& callback);
	
	template<class T, class _Ret, class ...Args>
	inline Job(std::shared_ptr<T> owner, _Ret(T::*method)(Args...), Args&&... args)
	{
		this->m_callback = [owner, method, args...]() { (owner.get()->*method)(args...); };
	}
	__forceinline void operator()() { this->m_callback(); }
private:
	CallbackType m_callback;
};

class JobTimer
{
	struct JobReserve
	{
		JobReserve() = default;
		JobReserve(uint64 tick, std::shared_ptr<Job> job)
			: reserveTick(tick), job(job) {}

		inline bool operator<(const JobReserve& other) const
		{
			return this->reserveTick < other.reserveTick;
		}

		uint64 reserveTick;
		std::shared_ptr<Job> job;
	};
public:
	void Reserve(uint64 tick, std::shared_ptr<Job> job);
	void Distribute(uint64 now);
private:
	ConcurrencyPriorityQueue<JobReserve> m_jobs;
	std::atomic<bool> m_isDistributed = false;
};

class JobQueue
{
public:
	void Push(std::shared_ptr<Job> job);
	void Flush();
	uint32 GetSize();
private:
	ConcurrencyQueue<std::shared_ptr<Job>> m_jobQue;
};

class JobSerializer : public std::enable_shared_from_this<JobSerializer>
{
	using CallbackType = std::function<void()>;
public:
	void Launch(CallbackType&& callback);
	void Launch(uint64 delay, CallbackType&& callback);
	
	template<uint64 _Dly = 0, class T, class _Ret, class... Args>
	inline void Launch(_Ret(T::*method)(Args...), Args... args)
	{
		auto owner = std::static_pointer_cast<T>(shared_from_this());
		if constexpr (_Dly)
		{
			auto job = Arena::MakeShared<Job>(owner, method, std::forward<Args>(args)...);
			if (auto* jobTimer = GEngine->GetJobTimer())
				jobTimer->Reserve(_Dly, job);
		}
		else GEngine->PushJob(Arena::MakeShared<Job>(owner, method, std::forward<Args>(args)...));
	}
	template<class T, class _Ret, class... Args>
	inline void Launch(int64 delay, _Ret(T::*method)(Args...), Args... args)
	{
		auto owner = std::static_pointer_cast<T>(shared_from_this());
		auto job = Arena::MakeShared<Job>(owner, method, std::forward<Args>(args)...);
		if (auto* jobTimer = GEngine->GetJobTimer())
			jobTimer->Reserve(delay, job);
	}
private:
	std::atomic<bool> m_isPushed;
};