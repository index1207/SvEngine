#pragma once

using JobCallback = std::function<void()>;

class Job
{
	USE_POOL(Job)
public:
	Job(JobCallback&& callback);
	
	template<class T, class _Ret, class ...Args>
	inline Job(std::shared_ptr<T> owner, _Ret(T::*method)(Args...), Args&&... args)
	{
		this->m_callback = [owner, method, args...]() { (owner.get()->*method)(args...); };
	}
	__forceinline void operator()() { this->m_callback(); }
private:
	JobCallback m_callback;
};

class JobTimer
{
	struct JobReserve
	{
		JobReserve() = default;
		JobReserve(uint64 tick, std::shared_ptr<class JobSerializer> serializer, std::shared_ptr<Job> job)
			: reserveTick(tick), serializer(serializer), job(job) {}

		inline bool operator<(const JobReserve& other) const
		{
			return this->reserveTick < other.reserveTick;
		}
		inline bool operator>(const JobReserve& other) const
		{
			return this->reserveTick > other.reserveTick;
		}

		uint64 reserveTick;
		std::weak_ptr<class JobSerializer> serializer;
		std::shared_ptr<Job> job;
	};
public:
	void Reserve(uint64 tick, std::shared_ptr<class JobSerializer> serializer, std::shared_ptr<Job> job);
	void Distribute(uint64 now);
private:
	ConcurrencyPriorityQueue<JobReserve, std::greater<JobReserve>> m_jobs;
};

class JobSerializer : public std::enable_shared_from_this<JobSerializer>
{
public:
	JobSerializer();
public:
	void Launch(JobCallback&& callback);
	void Launch(uint64 delay, JobCallback&& callback);
	
	template<uint64 _Dly = 0, class T, class _Ret, class... Args>
	inline void Launch(_Ret(T::*method)(Args...), Args... args)
	{
		auto owner = std::static_pointer_cast<T>(shared_from_this());
		auto job = MakeShared<Job>(owner, method, std::forward<Args>(args)...);
		if constexpr (_Dly)
		{
			if (auto* jobTimer = GEngine->GetJobTimer())
				jobTimer->Reserve(_Dly, shared_from_this(), job);
		}
		else m_jobs.push(job);
	}
	template<class T, class _Ret, class... Args>
	inline void Launch(int64 delay, _Ret(T::*method)(Args...), Args... args)
	{
		auto owner = std::static_pointer_cast<T>(shared_from_this());
		auto job = MakeShared<Job>(owner, method, std::forward<Args>(args)...);
		if (auto* jobTimer = GEngine->GetJobTimer())
			jobTimer->Reserve(delay, shared_from_this(), job);
	}

	void Push(std::shared_ptr<Job> job);
	void Flush();
private:
	ConcurrencyQueue<std::shared_ptr<Job>> m_jobs;
};