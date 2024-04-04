#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "ObjectUtils.h"
#include "Room.h"
#include "GameSession.h"
#include "Player.h"
#include "Thing.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	// TODO : DB���� Account ������ �ܾ�´�
	// TODO : DB���� ���� ���� �ܾ�´�
	Protocol::S_LOGIN loginPkt;

	for (int32 i = 0; i < 3; i++)
	{
		Protocol::ObjectInfo* player = loginPkt.add_players();
		Protocol::PositionInfo* posInfo = player->mutable_pos_info();

		posInfo->set_x(Utils::GetRandom(0.f, 100.f));
		posInfo->set_y(Utils::GetRandom(0.f, 100.f));
		posInfo->set_z(Utils::GetRandom(0.f, 100.f));
		posInfo->set_yaw(Utils::GetRandom(0.f, 45.f));
	}

	loginPkt.set_success(true);
	SEND_PACKET(loginPkt);

	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	// �÷��̾� ����
	PlayerRef player = ObjectUtils::CreatePlayer(static_pointer_cast<GameSession>(session));

	// �濡 ����
	GRoom->HandleEnterPlayerLocked(player);

	// ���� ����
	ThingRef thing = ObjectUtils::CreateThing();
	//GRoom->EnterRoom(thing);
	
	Protocol::S_O_SPAWN Opkt;

	Protocol::PositionInfo* posInfo = new Protocol::PositionInfo();
	posInfo->set_x(1.f);
	posInfo->set_y(2.f);
	posInfo->set_z(3.f);
	posInfo->set_yaw(4.f);
	posInfo->set_object_id(thing->posInfo->object_id());

	Opkt.set_allocated_objects(posInfo);
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(Opkt);


	GRoom->Broadcast(sendBuffer, player->objectInfo->object_id());


	return true;
}

bool Handle_C_LEAVE_GAME(PacketSessionRef& session, Protocol::C_LEAVE_GAME& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->HandleLeavePlayerLocked(player);

	return true;
}

bool Handle_C_MOVE(PacketSessionRef& session, Protocol::C_MOVE& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	// TODO

	room->HandleMoveLocked(pkt);

	return true;
}

bool Handle_C_O_MOVE(PacketSessionRef& session, Protocol::C_O_MOVE& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->HandleMoveLocked(pkt);

	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	return true;
}
