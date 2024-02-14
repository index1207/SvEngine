#include "pch.h"
#include "core/JobSerializer.hpp"

void JobSerializer::launch(CallbackType&& callback)
{
	push(std::make_shared<Job>(std::move(callback)));
}

void JobSerializer::push(std::shared_ptr<Job> jobPtr)
{
	auto prevCount = m_jobCount.fetch_add(1);
	m_jobQue.push(jobPtr);

	if (prevCount == 0)
	{
		if (LCurrentJobQueue == nullptr)
		{
			flush();
		}
		else
		{
			GGlobalQueue->Push(shared_from_this());
		}
	}
}

void JobSerializer::flush()
{
	LCurrentJobQueue = this;

	while (true)
	{
		std::vector<std::shared_ptr<Job>> jobs(m_jobQue.unsafe_begin(), m_jobQue.unsafe_end());
		m_jobQue.clear();

		const auto jobCount = static_cast<uint32>(jobs.size());
		for (uint32 i = 0; i < jobCount; ++i)
			(*jobs[jobCount])();
		
		if (m_jobCount.fetch_sub(jobCount) == jobCount)
		{
			LCurrentJobQueue = nullptr;
			return;
		}

		const auto now = GetTickCount64();
		if (now > LEndTickCount)
		{
			LCurrentJobQueue = nullptr;
			GGlobalQueue->Push(shared_from_this());
			return;
		}
	}
}

GlobalQueue::GlobalQueue()
{
}

GlobalQueue::~GlobalQueue()
{
}

void GlobalQueue::Push(std::shared_ptr<JobSerializer> jobSerializer)
{
	m_jobSerializerQue.push(jobSerializer);
}

std::shared_ptr<JobSerializer> GlobalQueue::Pop()
{
	std::shared_ptr<JobSerializer> ret = nullptr;
	if (m_jobSerializerQue.try_pop(ret))
		return ret;
	return nullptr;
}
