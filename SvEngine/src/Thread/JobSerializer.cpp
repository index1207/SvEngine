#include "pch.h"
#include "Thread/JobSerializer.hpp"

void JobSerializer::Launch(JobCallback&& callback)
{
	m_jobs.push(MakeShared<Job>(std::move(callback)));
}

void JobSerializer::Launch(uint64 delay, JobCallback&& callback)
{
	auto job = MakeShared<Job>(std::move(callback));
	if (auto* jobTimer = GEngine->GetJobTimer())
		jobTimer->Reserve(delay, shared_from_this(), job);
}

void JobSerializer::Push(std::shared_ptr<Job> job)
{
	m_jobs.push(job);
}

void JobSerializer::Flush()
{
}

void JobTimer::Reserve(uint64 tick, std::shared_ptr<class JobSerializer> serializer, std::shared_ptr<Job> job)
{
	m_jobs.push({ GetTickCount64() + tick, serializer, job });
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
		if (auto serializer = job.serializer.lock())
			serializer->Push(job.job);
	}

	m_isDistributed.store(false);
}

CREATE_ARENA(Job, 1024)
Job::Job(JobCallback&& callback) : m_callback(std::forward<JobCallback>(callback))
{
}
