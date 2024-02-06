#include "pch.h"
#include "Room.hpp"
#include "Player.hpp"
#include "main/GameSession.hpp"

#include "generated/Protocol.gen.hpp"

#include <format>

std::shared_ptr<Room> GRoom = std::make_shared<Room>();

Room::Room()
{
}

Room::~Room()
{
}

bool Room::HandleEnterGameLocked(std::shared_ptr<Player> player)
{
	std::lock_guard lock(mtx);
	
	auto success = EnterPlayer(player);

	player->playerInfo->x = action::Random::RandomRange(-100.f, 100.f);
	player->playerInfo->y = action::Random::RandomRange(-100.f, 100.f);
	player->playerInfo->z = 0;
	player->playerInfo->yaw = action::Random::RandomRange(-180.f, 180.f);


	Console::Log(std::format("[{}, {}, {}]", player->playerInfo->x, player->playerInfo->y, player->playerInfo->z));

	/* 나에게 입장 알림 */
	{
		gen::EnterGameRes enterGame;
		enterGame.isSuccess = success;
		enterGame.player = *player->playerInfo;

		if (auto session = player->session.lock())
			session->send(&enterGame);
	}

	/* 다른 플레이어에게 알림 */
	{
		gen::SpawnNotify spawnNotify;
		spawnNotify.playerList.push_back(*player->playerInfo);
		Broadcast(&spawnNotify, player->playerInfo->objectId);
	}

	/* 나에게 다른 플레이어 알림 */
	{
		gen::SpawnNotify spawnNotify;
		for (const auto& item : _players)
			spawnNotify.playerList.push_back(*item.second->playerInfo);
		if (auto session = player->session.lock())
			session->send(&spawnNotify);
	}
	

	return success;
}

bool Room::EnterPlayer(std::shared_ptr<Player> player)
{
	if (_players.find(player->playerInfo->objectId) != _players.end())
		return false;

	_players.insert({ player->playerInfo->objectId, player });
	player->room.store(GRoom->shared_from_this());
	return true;
}

void Room::Broadcast(Packet* pk, uint64 exceptId)
{
	for (const auto& item : _players)
	{
		auto player = item.second;
		if (player->playerInfo->objectId == exceptId)
			continue;
		if (auto session = player->session.lock())
			session->send(pk);
	}
}
