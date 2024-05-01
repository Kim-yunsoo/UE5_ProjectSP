#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"
#include "ObjectUtils.h"
#include "Thing.h"

RoomRef GRoom = make_shared<Room>();

Room::Room()
{
	//createAllObject();

}

Room::~Room()
{

}

bool Room::EnterRoom(ObjectRef object, bool randPos)
{
	WRITE_LOCK;

	bool success = EnterObject(object);



	// 입장 사실을 신입 플레이어에게 알린다
	if(auto player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_ENTER_GAME enterGamePkt;
		enterGamePkt.set_success(success);

		Protocol::PlayerInfo* playerInfo = new Protocol::PlayerInfo();
		playerInfo->CopyFrom(*player->objectInfo);
		enterGamePkt.set_allocated_player(playerInfo);
		//enterGamePkt.release_player();

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// 입장 사실을 다른 플레이어에게 알린다
	{
		Protocol::S_SPAWN spawnPkt;

		Protocol::PlayerInfo* objectInfo = spawnPkt.add_players();
		objectInfo->CopyFrom(*object->objectInfo);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		Broadcast(sendBuffer, object->objectInfo->object_id());
	}

	// 기존 입장한 플레이어 목록을 신입 플레이어한테 전송해준다
	if (auto player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_SPAWN spawnPkt;

		for (auto& item : _player)
		{
			if (item.second->IsPlayer() == false)		// 플레이어한테만 전송. 물건들한테는 보낼 필요 없으니까!
				continue;

			Protocol::PlayerInfo* playerInfo = spawnPkt.add_players();
			playerInfo->CopyFrom(*item.second->objectInfo);
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	return success;
}

bool Room::LeaveRoom(ObjectRef object)
{
	if (object == nullptr)
		return false;

	WRITE_LOCK;

	const uint64 objectId = object->objectInfo->object_id();
	bool success = LeaveObject(objectId);

	// 퇴장 사실을 퇴장하는 플레이어에게 알린다
	if (auto player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_LEAVE_GAME leaveGamePkt;

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(leaveGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// 퇴장 사실을 알린다
	{
		Protocol::S_DESPAWN despawnPkt;
		despawnPkt.add_object_ids(objectId);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(despawnPkt);
		Broadcast(sendBuffer, objectId);
		if (auto player = dynamic_pointer_cast<Player>(object))
			if (auto session = player->session.lock())
				session->Send(sendBuffer);
	}

	return success;
}

bool Room::HandleEnterPlayerLocked(PlayerRef player)
{
	WRITE_LOCK;

	bool success = EnterObject(player);



	// 입장 사실을 신입 플레이어에게 알린다
	{
		Protocol::S_ENTER_GAME enterGamePkt;
		enterGamePkt.set_success(success);

		Protocol::PlayerInfo* playerInfo = new Protocol::PlayerInfo();
		playerInfo->CopyFrom(*player->objectInfo);
		enterGamePkt.set_allocated_player(playerInfo);
		//enterGamePkt.release_player();

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// 입장 사실을 다른 플레이어에게 알린다
	{
		Protocol::S_SPAWN spawnPkt;

		Protocol::PlayerInfo* playerInfo = spawnPkt.add_players();
		playerInfo->CopyFrom(*player->objectInfo);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		Broadcast(sendBuffer, player->objectInfo->object_id());
	}

	// 기존 입장한 플레이어 목록을 신입 플레이어한테 전송해준다
	{
		Protocol::S_SPAWN spawnPkt;

		for (auto& item : _player)
		{
			if(item.second->IsPlayer() == false)		// 플레이어 정보만 보내줌. 물건들 정보는 뒤에서 따로
				continue;

			Protocol::PlayerInfo* playerInfo = spawnPkt.add_players();
			playerInfo->CopyFrom(*item.second->objectInfo);
		}

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

		//// 맵에 배치될 물건들 정보 전송


	return success;
	//return EnterRoom(player, true);
}


bool Room::HandleLeavePlayerLocked(PlayerRef player)
{
	if (player == nullptr)
		return false;

	WRITE_LOCK;

	const uint64 objectId = player->objectInfo->object_id();
	bool success = LeaveObject(objectId);

	// 퇴장 사실을 퇴장하는 플레이어에게 알린다
	{
		Protocol::S_LEAVE_GAME leaveGamePkt;

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(leaveGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// 퇴장 사실을 알린다
	{
		Protocol::S_DESPAWN despawnPkt;
		despawnPkt.add_object_ids(objectId);

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(despawnPkt);
		Broadcast(sendBuffer, objectId);

		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	return success;
	//return LeaveRoom(player);
}





bool Room::EnterObject(ObjectRef object)
{
	// 있다면 문제가 있다.
	if (_player.find(object->objectInfo->object_id()) != _player.end())
		return false;

	_player.insert(make_pair(object->objectInfo->object_id(), object));
	
	object->room.store(shared_from_this());

	return true;
}


bool Room::LeaveObject(uint64 objectId)
{
	// 없다면 문제가 있다.
	if (_player.find(objectId) == _player.end())
		return false;

	ObjectRef object = _player[objectId];
	object->room.store(weak_ptr<Room>());

	_player.erase(objectId);

	return true;
}

void Room::Broadcast(SendBufferRef sendBuffer, uint64 exceptId)
{	// 플레이어한테만 전송
	for (auto& item : _player)
	{
		PlayerRef player = dynamic_pointer_cast<Player>(item.second);		// 플레이어로 바꿔서 얘네한테만 처리
		if(player == nullptr)
			continue;

		if (player->objectInfo->object_id() == exceptId)
			continue;

		if (GameSessionRef session = player->session.lock())
			session->Send(sendBuffer);
	}
}

void Room::ObjectBroadcast(SendBufferRef sendBuffer)
{	// 플레이어한테만 전송
	for (auto& item : _player)
	{
		PlayerRef player = dynamic_pointer_cast<Player>(item.second);		// 플레이어로 바꿔서 얘네한테만 처리
		if (player == nullptr)
			continue;

		if (GameSessionRef session = player->session.lock())
			session->Send(sendBuffer);
	}
}