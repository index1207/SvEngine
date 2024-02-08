#pragma once

#include <unordered_map>
#include <mutex>
#include <memory>

#include "generated/Struct.gen.hpp"

class Player;

class Room : public std::enable_shared_from_this<Room>
{
public:
	Room();
	virtual ~Room();
public:
	bool HandleEnterGameLocked(std::shared_ptr<Player> player);
	bool HandleLeaveGameLocked(std::shared_ptr<Player> player);
	void HandleMoveLocked(std::shared_ptr<Player> player, gen::Status status);
private:
	bool EnterPlayer(std::shared_ptr<Player> player);
	bool LeavePlayer(uint64 player);
	void Broadcast(Packet* pk, uint64 exceptId = 0);
private:
	std::mutex _lock;
	std::unordered_map<uint64_t, std::shared_ptr<Player>> _players;
};

extern std::shared_ptr<Room> GRoom;