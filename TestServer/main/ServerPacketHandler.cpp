#include "pch.h"
#include "generated/ServerPacketHandler.gen.hpp"

#include "GameSession.hpp"
#include "Player.hpp"
#include "Room.hpp"

#include <util/ObjectUtils.hpp>

using namespace gen;

bool gen::PacketHandler::LoginReqPacketHandler(TSharedPtr<Session> session, TSharedPtr<LoginReq> packet)
{
	// TODO: DB에서 Account, 유저 정보 긁어오기
	LoginRes loginPkt;
	loginPkt.isSuccess = true;

	session->send(&loginPkt);

	return true;
}

bool gen::PacketHandler::EnterGameReqPacketHandler(TSharedPtr<Session> session, TSharedPtr<EnterGameReq> packet)
{
	auto player = ObjectUtils::CreatePlayer(session);
	GRoom->HandleEnterGameLocked(player);

	return true;
}

bool gen::PacketHandler::LeaveGameReqPacketHandler(TSharedPtr<Session> session, TSharedPtr<LeaveGameReq> packet)
{
	auto gameSession = std::static_pointer_cast<GameSession>(session);
	
	auto player = gameSession->player.load();
	if (packet == nullptr)
		return false;

	auto room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->HandleLeaveGameLocked(player);
	
	return true;
}

bool gen::PacketHandler::MoveReqPacketHandler(TSharedPtr<Session> session, TSharedPtr<MoveReq> packet)
{
	auto gameSession = std::static_pointer_cast<GameSession>(session);

	auto player = gameSession->player.load();
	if (packet == nullptr)
		return false;

	auto room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->HandleMoveLocked(player, packet->status);

	return true;
}
