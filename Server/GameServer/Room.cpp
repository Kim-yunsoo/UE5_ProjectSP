#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"
#include "ObjectUtils.h"
#include "Thing.h"

LobbyInfomation G_LobbyInfo;
LobbyRef GRoom = make_shared<Lobby>();

Lobby::Lobby()
{
	//createAllObject();
	for (int i = 0; i < 8; i++) {
		G_LobbyInfo.publicBoard[i].membership_id="";
		G_LobbyInfo.publicBoard[i].msg = "";
		G_LobbyInfo.publicBoard[i].msg_num = "";
	}

	// �б� �Խ��� ���� �ʱ�ȭ
	for (int i = 0; i < 3; i++) {
		G_LobbyInfo.privateBoard[i].membership_id = "";
		G_LobbyInfo.privateBoard[i].msg = "";
	}
		G_LobbyInfo.privateBoard[0].membership_type = Protocol::SchoolType::SCHOOL_TYPE_GREEN;
		G_LobbyInfo.privateBoard[1].membership_type = Protocol::SchoolType::SCHOOL_TYPE_PURPLE;
		G_LobbyInfo.privateBoard[2].membership_type = Protocol::SchoolType::SCHOOL_TYPE_ORANGE;

}

Lobby::~Lobby()
{

}

bool Lobby::EnterRoom(ObjectRef object, bool randPos)
{
	WRITE_LOCK;

	bool success = EnterPlayer(object);



	//// ���� ����� ���� �÷��̾�� �˸���
	//if(auto player = dynamic_pointer_cast<Player>(object))
	//{
	//	Protocol::S_ENTER_GAME enterGamePkt;
	//	enterGamePkt.set_success(success);

	//	Protocol::PlayerInfo* playerInfo = new Protocol::PlayerInfo();
	//	playerInfo->CopyFrom(*player->objectInfo);
	//	enterGamePkt.set_allocated_player(playerInfo);
	//	//enterGamePkt.release_player();

	//	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
	//	if (auto session = player->session.lock())
	//		session->Send(sendBuffer);
	//}

	//// ���� ����� �ٸ� �÷��̾�� �˸���
	//{
	//	Protocol::S_SPAWN spawnPkt;

	//	Protocol::PlayerInfo* objectInfo = spawnPkt.add_players();
	//	objectInfo->CopyFrom(*object->objectInfo);

	//	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
	//	Broadcast(sendBuffer, object->objectInfo->object_id());
	//}

	//// ���� ������ �÷��̾� ����� ���� �÷��̾����� �������ش�
	//if (auto player = dynamic_pointer_cast<Player>(object))
	//{
	//	Protocol::S_SPAWN spawnPkt;

	//	for (auto& item : _player)
	//	{
	//		if (item.second->IsPlayer() == false)		// �÷��̾����׸� ����. ���ǵ����״� ���� �ʿ� �����ϱ�!
	//			continue;

	//		Protocol::PlayerInfo* playerInfo = spawnPkt.add_players();
	//		playerInfo->CopyFrom(*item.second->objectInfo);
	//	}

	//	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
	//	if (auto session = player->session.lock())
	//		session->Send(sendBuffer);
	//}

	return success;
}

bool Lobby::LeaveRoom(ObjectRef object)
{
	if (object == nullptr)
		return false;

	WRITE_LOCK;

	const uint64 objectId = object->objectInfo->object_id();
	bool success = LeaveObject(objectId);

	// ���� ����� �����ϴ� �÷��̾�� �˸���
	if (auto player = dynamic_pointer_cast<Player>(object))
	{
		Protocol::S_LEAVE_GAME leaveGamePkt;

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(leaveGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// ���� ����� �˸���
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

bool Lobby::HandleEnterPlayerLocked(PlayerRef player)
{
	WRITE_LOCK;

	bool success = EnterPlayer(player);



	//// ���� ����� ���� �÷��̾�� �˸���
	//{
	//	Protocol::S_ENTER_GAME enterGamePkt;
	//	enterGamePkt.set_success(success);

	//	Protocol::PlayerInfo* playerInfo = new Protocol::PlayerInfo();
	//	playerInfo->CopyFrom(*player->objectInfo);
	//	enterGamePkt.set_allocated_player(playerInfo);
	//	//enterGamePkt.release_player();

	//	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
	//	if (auto session = player->session.lock())
	//		session->Send(sendBuffer);
	//}

	//// ���� ����� �ٸ� �÷��̾�� �˸���
	//{
	//	Protocol::S_SPAWN spawnPkt;

	//	Protocol::PlayerInfo* playerInfo = spawnPkt.add_players();
	//	playerInfo->CopyFrom(*player->objectInfo);

	//	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
	//	Broadcast(sendBuffer, player->objectInfo->object_id());
	//}

	//// ���� ������ �÷��̾� ����� ���� �÷��̾����� �������ش�
	//{
	//	Protocol::S_SPAWN spawnPkt;

	//	for (auto& item : _player)
	//	{
	//		if(item.second->IsPlayer() == false)		// �÷��̾� ������ ������. ���ǵ� ������ �ڿ��� ����
	//			continue;

	//		Protocol::PlayerInfo* playerInfo = spawnPkt.add_players();
	//		playerInfo->CopyFrom(*item.second->objectInfo);
	//	}

	//	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
	//	if (auto session = player->session.lock())
	//		session->Send(sendBuffer);
	//}

	//	//// �ʿ� ��ġ�� ���ǵ� ���� ����


	return success;
	//return EnterRoom(player, true);
}


bool Lobby::HandleLeavePlayerLocked(PlayerRef player)
{
	if (player == nullptr)
		return false;

	WRITE_LOCK;

	const uint64 objectId = player->objectInfo->object_id();
	bool success = LeaveObject(objectId);

	// ���� ����� �����ϴ� �÷��̾�� �˸���
	{
		Protocol::S_LEAVE_GAME leaveGamePkt;

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(leaveGamePkt);
		if (auto session = player->session.lock())
			session->Send(sendBuffer);
	}

	// ���� ����� �˸���
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





bool Lobby::EnterPlayer(ObjectRef object)
{
	// �ִٸ� ������ �ִ�.
	if (_player.find(object->objectInfo->object_id()) != _player.end())
		return false;

	_player.insert(make_pair(object->objectInfo->object_id(), object));
	
	object->lobby.store(shared_from_this());

	return true;
}


bool Lobby::LeaveObject(uint64 objectId)
{
	// ���ٸ� ������ �ִ�.
	if (_player.find(objectId) == _player.end())
		return false;

	ObjectRef object = _player[objectId];
	object->lobby.store(weak_ptr<Lobby>());

	_player.erase(objectId);

	return true;
}

void Lobby::Broadcast(SendBufferRef sendBuffer, uint64 exceptId)
{	// �÷��̾����׸� ����
	for (auto& item : _player)
	{
		PlayerRef player = dynamic_pointer_cast<Player>(item.second);		// �÷��̾�� �ٲ㼭 ������׸� ó��
		if(player == nullptr)
			continue;

		if (player->objectInfo->object_id() == exceptId)
			continue;

		if (GameSessionRef session = player->session.lock())
			session->Send(sendBuffer);
	}
}

void Lobby::ObjectBroadcast(SendBufferRef sendBuffer)
{	// �÷��̾����׸� ����
	for (auto& item : _player)
	{
		PlayerRef player = dynamic_pointer_cast<Player>(item.second);		// �÷��̾�� �ٲ㼭 ������׸� ó��
		if (player == nullptr)
			continue;

		if (GameSessionRef session = player->session.lock())
			session->Send(sendBuffer);
	}
}