#pragma once

#include <memory>

namespace sv { class Session; }

class ObjectUtils
{
public:
	static std::shared_ptr<class Player> CreatePlayer(std::shared_ptr<sv::Session> session);
private:
	static std::atomic<int64_t> s_idGenerator;
};

