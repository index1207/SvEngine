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

		gen::PacketId id = (gen::PacketId)0;
		std::memcpy(&id, buffer.data(), sizeof(unsigned short));
		id = static_cast<gen::PacketId>(htons(static_cast<unsigned short>(id)));

		gen::PacketHandler::onReceivePacket(shared_from_this(), id, buffer);
	}
public:
	std::atomic<std::shared_ptr<class Player>> player;
};