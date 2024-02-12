#pragma once

#include "generated/ServerPacketHandler.gen.hpp"

class GameSession : public sv::Session
{
public:
	GameSession() {}
	~GameSession() {}
public:
	virtual void onConnected() override
	{
		Console::Log("Connected");
	}

	virtual void onDisconnected() override
	{
		Console::Log("Disconnected");
	}

	virtual void onReceive(std::span<char> buffer, int length) override
	{
		if (length <= 2)
			return;

		auto res = gen::PacketHandler::handlePacket(shared_from_this(), buffer);
	}
public:
	std::queue<std::shared_ptr<Packet>> packetQue;
	std::atomic<std::shared_ptr<class Player>> player;
};