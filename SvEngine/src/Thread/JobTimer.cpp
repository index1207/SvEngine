#include "pch.h"
#include "Thread/JobTimer.hpp"

#include "Thread/JobSerializer.hpp"

void JobTimer::Reserve(uint64 tick, std::weak_ptr<JobSerializer> owner, std::shared_ptr<Job> job)
{
	const auto executeTick = GetTickCount64() + tick;

	m_items.push({ executeTick, new JobData(owner, job) });
}

void JobTimer::Distribute(uint64 now)
{
	if (m_distributing.exchange(true))
		return;

	ConcurrencyVector<TimerItem> items;
	while (!m_items.empty())
	{
		TimerItem timer;
		if (m_items.try_pop(timer))
		{
			if (now < timer.executeTick)
			{
				m_items.push(timer);
				break;
			}

			items.push_back(timer);
		}
	}

	for (auto& item : items)
	{
		if (const auto owner = item.jobData->owner.lock())
			owner->Push(item.jobData->job, true);

		delete item.jobData;
	}

	m_distributing.store(false);
}

void JobTimer::Clear()
{
	while (!m_items.empty())
	{
		TimerItem jobData;
		if (m_items.try_pop(jobData))
			if (jobData.jobData) delete jobData.jobData;
	}
}
