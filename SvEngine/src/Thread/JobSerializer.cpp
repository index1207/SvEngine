#include "pch.h"
#include "Thread/JobSerializer.hpp"

void JobSerializer::Launch(CallbackType callback)
{
	Push(std::make_shared<Job>(std::move(callback)));
}

void JobSerializer::Launch(uint64 delay, CallbackType&& callback)
{
	auto job = std::make_shared<Job>(std::move(callback));
	if (auto* jobTimer = GEngine->GetJobTimer())
		jobTimer->Reserve(delay, shared_from_this(), job);
}

void JobSerializer::Push(std::shared_ptr<Job> jobPtr, bool pushOnly)
{
	auto prevCount = m_jobCount.fetch_add(1);
	m_jobQue.push(jobPtr);

	if (prevCount == 0)
	{
		if (LCurrentJobQueue == nullptr && !pushOnly)
		{
			Flush();
		}
		else
		{
			GEngine->GetJobQueue()->Push(shared_from_this());
		}
	}
}

void JobSerializer::Flush()
{
	LCurrentJobQueue = this;

	while (true)
	{
		std::vector<std::shared_ptr<Job>> jobs(m_jobQue.unsafe_begin(), m_jobQue.unsafe_end());
		m_jobQue.clear();

		const auto jobCount = static_cast<uint32>(jobs.size());
		for (uint32 i = 0; i < jobCount; ++i)
			(*jobs[i])();
		
		if (m_jobCount.fetch_sub(jobCount) == jobCount)
		{
			LCurrentJobQueue = nullptr;
			return;
		}

		const auto now = GetTickCount64();
		if (now > LEndTickCount)
		{
			LCurrentJobQueue = nullptr;
			GEngine->GetJobQueue()->Push(shared_from_this());
			return;
		}
	}
}

JobQueue::JobQueue()
{
}

JobQueue::~JobQueue()
{
}

void JobQueue::Push(std::shared_ptr<JobSerializer> jobSerializer)
{
	m_jobSerializerQue.push(jobSerializer);
}

std::shared_ptr<JobSerializer> JobQueue::Pop()
{
	std::shared_ptr<JobSerializer> ret = nullptr;
	if (m_jobSerializerQue.try_pop(ret))
		return ret;
	return nullptr;
}