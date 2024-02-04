#include "pch.h"
#include "ObjectUtils.hpp"

#include "main/GameSession.hpp"
#include "content/Player.hpp"

std::atomic<int64_t> ObjectUtils::s_idGenerator = 0;

std::shared_ptr<class Player> ObjectUtils::CreatePlayer(std::shared_ptr<Session> session)
{
	auto gameSession = std::static_pointer_cast<GameSession>(session);
	const auto newId = s_idGenerator.fetch_add(1);

	auto player = std::make_shared<Player>();
	player->playerInfo->objectId = newId;

	player->session = gameSession;
	gameSession->player.store(player);

	return player;
}
