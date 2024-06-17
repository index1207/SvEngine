#include "pch.h"
#include "Thread/JobSerializer.hpp"

JobSerializer::JobSerializer()
{
	GEngine->AddSerializer(this);
}

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
	while (!m_jobs.empty())
	{
		std::shared_ptr<Job> job;
		if (m_jobs.try_pop(job))
			(*job)();
	}
	GEngine->AddSerializer(this);
}

void JobTimer::Reserve(uint64 tick, std::shared_ptr<class JobSerializer> serializer, std::shared_ptr<Job> job)
{
	m_jobs.push({ GetTickCount64() + tick, serializer, job });
}

void JobTimer::Distribute(uint64 now)
{
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
			else if (auto serializer = jobReserve.serializer.lock())
				serializer->Push(jobReserve.job);
		}
	}
}

Job::Job(JobCallback&& callback) : m_callback(std::forward<JobCallback>(callback))
{
}
