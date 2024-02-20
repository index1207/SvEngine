#pragma once

class Job;

struct JobData
{
	JobData(std::weak_ptr<JobSerializer> owner, std::shared_ptr<Job> job) : owner(owner), job(job) { }
	std::weak_ptr<JobSerializer> owner;
	std::shared_ptr<Job> job;
};

struct TimerItem
{
	bool operator<(const TimerItem& other) const
	{
		return executeTick < other.executeTick;
	}
	uint64 executeTick = 0;
	JobData* jobData = nullptr;
};

class JobTimer
{
public:
	void Reserve(uint64 tick, std::weak_ptr<JobSerializer> owner, std::shared_ptr<Job> job);
	void Distribute(uint64 now);
	void Clear();
private:
	ConcurrencyPriorityQueue<TimerItem> m_items;
	std::atomic<bool> m_distributing = false;
};