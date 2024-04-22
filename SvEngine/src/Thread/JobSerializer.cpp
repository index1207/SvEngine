#include "pch.h"
#include "Thread/JobSerializer.hpp"

void JobSerializer::Launch(CallbackType&& callback)
{
	GEngine->PushJob(Arena::MakeShared<Job>(std::move(callback)));
}

void JobSerializer::Launch(uint64 delay, CallbackType&& callback)
{
	auto job = Arena::MakeShared<Job>(std::move(callback));
	if (auto* jobTimer = GEngine->GetJobTimer())
		jobTimer->Reserve(delay, job);
}

void JobTimer::Reserve(uint64 tick, std::shared_ptr<Job> job)
{
	m_jobs.push({ GetTickCount64() + tick, job });
}

void JobTimer::Distribute(uint64 now)
{
	if (m_isDistributed.exchange(true) == true) // 만약 Job을 빼고 있다면
		return;

	Vector<JobReserve> executeJobs;
	while (!m_jobs.empty())
	{
		JobReserve jobReserve;
		if (m_jobs.try_pop(jobReserve))
		{
			if (now < jobReserve.reserveTick)
			{
				m_jobs.push(jobReserve);
				break;
			}
			executeJobs.push_back(jobReserve);
		}
	}
	for (const auto& job : executeJobs)
	{
		GEngine->PushJob(job.job);
	}

	m_isDistributed.store(false);
}

void JobQueue::Push(std::shared_ptr<Job> job)
{
	m_jobQue.push(job);
}

void JobQueue::Flush()
{
	while (!m_jobQue.empty())
	{
		std::shared_ptr<Job> job;
		if (m_jobQue.try_pop(job))
		{
			(*job)();
		}
	}
}

uint32 JobQueue::GetSize()
{
	return m_jobQue.unsafe_size();
}


CREATE_DYNAMIC_ARENA(Job, 1024)
Job::Job(CallbackType&& callback) : m_callback(std::move(callback))
{
}
