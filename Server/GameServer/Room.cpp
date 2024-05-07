#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "GameSession.h"
#include "ObjectUtils.h"
#include "Thing.h"

LobbyInformation G_LobbyInfo;
LobbyRef GLobby = make_shared<Lobby>();
vector<RoomInformation> RoomList;

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

bool Lobby::EnterLobbyLocked(PlayerRef player)
{
	WRITE_LOCK;

	bool success = EnterLobby(player);

	return success;
}


bool Lobby::LeaveLobbyLocked(PlayerRef player)
{
	if (player == nullptr)
		return false;

	WRITE_LOCK;

	const uint64 objectId = player->objectInfo->object_id();
	bool success = LeaveLobby(objectId);

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

bool Lobby::EnterLobby(ObjectRef object)
{
	// �ִٸ� ������ �ִ�.
	if (_player.find(object->objectInfo->object_id()) != _player.end())
		return false;

	_player.insert(make_pair(object->objectInfo->object_id(), object));
	
	
	object->lobby.store(shared_from_this());

	return true;
}


bool Lobby::LeaveLobby(uint64 objectId)
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

void Lobby::RoomSend(SendBufferRef sendBuffer, const string* membership_id)
{
	// �뿡 �ִ� ���̵� ���ؼ��� ����
	//for (auto& item : _player)
	//{
	//	cout<<"id : " << *item.second->player_id << endl;
	//	
	//}

	//cout << "id : " << *(_player[1]->player_id) << endl;
	

}

