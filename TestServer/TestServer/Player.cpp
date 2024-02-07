#include "pch.h"
#include "Player.hpp"

Player::Player()
{
	playerInfo = new gen::PlayerInfo;
}

Player::~Player()
{
	delete playerInfo;
}
