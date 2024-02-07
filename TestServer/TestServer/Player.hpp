#pragma once

#include <generated/Struct.gen.hpp>

class GameSession;
class Room;

class Player : public std::enable_shared_from_this<Player>
{
public:
	Player();	
	virtual ~Player();
public:
	gen::PlayerInfo* playerInfo;
	std::weak_ptr<GameSession> session;
	std::atomic<std::weak_ptr<Room>> room;
};

