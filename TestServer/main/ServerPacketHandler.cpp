#include "pch.h"
#include "generated/ServerPacketHandler.gen.hpp"

#include "GameSession.hpp"
#include "Player.hpp"
#include "Room.hpp"

#include <util/ObjectUtils.hpp>

using namespace gen;

void gen::PacketHandler::LoginReqPacketHandler(TSharedPtr<Session> session, TSharedPtr<LoginReq> packet)
{
	// TODO: DB에서 Account, 유저 정보 긁어오기

	LoginRes loginPkt;
	for (int i = 0; i < 3; ++i) {
		PlayerInfo info;
		info.location.x = action::Random::RandomRange(-100.f, 100.f);
		info.location.y = action::Random::RandomRange(-100.f, 100.f);
		info.location.z = 0;
		info.location.yaw = action::Random::RandomRange(-180.f, 180.f);
		loginPkt.playerList.push_back(info);
	}
	loginPkt.isSuccess = true;

	session->send(&loginPkt);

	return;
}

void gen::PacketHandler::EnterGameReqPacketHandler(TSharedPtr<Session> session, TSharedPtr<EnterGameReq> packet)
{
	auto player = ObjectUtils::CreatePlayer(session);
	GRoom->HandleEnterGameLocked(player);

	return;
}

void gen::PacketHandler::LeaveGameReqPacketHandler(TSharedPtr<Session> session, TSharedPtr<LeaveGameReq> packet)
{
	auto gameSession = std::static_pointer_cast<GameSession>(session);
	
	auto player = gameSession->player.load();
	if (packet == nullptr)
		return;

	auto room = player->room.load().lock();
	if (room == nullptr)
		return;

	room->HandleLeaveGameLocked(player);
}
