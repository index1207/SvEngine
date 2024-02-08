#include "pch.h"
#include "Room.hpp"
#include "Player.hpp"
#include "GameSession.hpp"

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
	std::lock_guard lock(_lock);
	
	auto success = EnterPlayer(player);

	player->playerInfo->status.location.x = action::Random::RandomRange(-100.f, 100.f) * 3;
	player->playerInfo->status.location.y = action::Random::RandomRange(-100.f, 100.f) * 3;
	player->playerInfo->status.location.z = 100;
	player->playerInfo->status.yaw = action::Random::RandomRange(-180.f, 180.f);

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

bool Room::HandleLeaveGameLocked(std::shared_ptr<Player> player)
{
	if (player == nullptr)
		return false;

	std::lock_guard lock(_lock);
	auto objectId = player->playerInfo->objectId;
	bool success = LeavePlayer(objectId);

	/* 나에게 퇴장 알림 */
	{
		gen::LeaveGameRes leaveRes;
		if (auto session = player->session.lock())
			session->send(&leaveRes);
	}

	/* 다른 플레이어에게 알림 */
	{
		gen::DespawnNotify despawn;
		despawn.playerList.push_back(objectId);
		Broadcast(&despawn, player->playerInfo->objectId);

		if (auto session = player->session.lock())	
			session->send(&despawn);
	}

	return success;
}

void Room::HandleMoveLocked(std::shared_ptr<Player> player, gen::Status status)
{
	std::lock_guard lock(_lock);

	const auto objectId = player->playerInfo->objectId;
	if (_players.find(objectId) == _players.end())
		return;

	gen::PlayerInfo info;
	info.objectId = objectId;
	info.status = status;

	gen::MoveRes moveRes;
	moveRes.info = info;
	Broadcast(&moveRes);
}

bool Room::EnterPlayer(std::shared_ptr<Player> player)
{
	if (_players.find(player->playerInfo->objectId) != _players.end())
		return false;

	_players.insert({ player->playerInfo->objectId, player });
	player->room.store(GRoom->shared_from_this());
	return true;
}

bool Room::LeavePlayer(uint64 objectId)
{
	if (_players.find(objectId) == _players.end())
		return false;

	auto player = _players[objectId];
	player->room.store(std::weak_ptr<Room>());

	_players.erase(objectId);

	return false;
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
