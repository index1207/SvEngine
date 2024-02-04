#include "pch.h"
#include "generated/ServerPacketHandler.gen.hpp"

#include "content/Room.hpp"

#include <format>

using namespace gen;

void gen::PacketHandler::LoginReqPacketHandler(TSharedPtr<Session> session, TSharedPtr<LoginReq> packet)
{
	// TODO: DB에서 Account, 유저 정보 긁어오기

	LoginRes loginPkt;
	for (int i = 0; i < 3; ++i) {
		PlayerInfo info;
		info.x = action::Random::RandomRange(-100.f, 100.f);
		info.y = action::Random::RandomRange(-100.f, 100.f);
		info.z = 0;
		info.yaw = action::Random::RandomRange(-180.f, 180.f);
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
