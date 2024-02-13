#pragma once
class JobSerializer
{
	using Callback = std::function<void()>;
public:
	void launch(Callback&& callback);
private:
	ConcurrencyQueue<Callback> m_jobQue;
};