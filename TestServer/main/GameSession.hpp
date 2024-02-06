#pragma once

#include "generated/ServerPacketHandler.gen.hpp"
#include <queue>

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

		gen::PacketId id = gen::PacketId::None;
		std::memcpy(&id, buffer.data(), sizeof(unsigned short));
		id = (gen::PacketId)ntohs((u_short)id);

		auto packet = gen::PacketHandler::parsePacket(id, buffer);
		if (packet != nullptr)
			packet->handler(shared_from_this());
	}
	void flush()
	{
		while (!packetQue.empty())
		{
			auto packet = packetQue.front();
			if (packet != nullptr)
				packet->handler(shared_from_this());
			packetQue.pop();
		}
	}
public:
	std::queue<std::shared_ptr<Packet>> packetQue;
	std::atomic<std::shared_ptr<class Player>> player;
};