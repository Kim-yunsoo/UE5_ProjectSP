#pragma once

#include <array>
#include <string>

class publicchatInfo
{
public:
	string membership_id;
	string msg;
	string msg_num;

};

class privatechatInfo
{
public:
	string membership_id;
	string msg;
	Protocol::SchoolType membership_type;
};


class LobbyInfomation
{
public:
	// �ۺ� �Խ��� ����
	array <publicchatInfo, 8> publicBoard;

	// �����̺� �Խ��� ����
	array <privatechatInfo, 3> privateBoard;

	// ��Ī�� ����

};


class Room : public enable_shared_from_this<Room>
{
public:
	Room();
	virtual ~Room();

public:
	bool EnterRoom(ObjectRef object, bool randPos = true);
	bool LeaveRoom(ObjectRef object);


	bool HandleEnterPlayerLocked(PlayerRef player);
	bool HandleLeavePlayerLocked(PlayerRef player);




private:
	bool EnterObject(ObjectRef object);
	bool LeaveObject(uint64 objectId);

	USE_LOCK;

public:
	void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);
	void ObjectBroadcast(SendBufferRef sendBuffer);

//private:
public:
	unordered_map<uint64, ObjectRef> _player;
	unordered_map<uint64, ObjectRef> _objects;
};

extern RoomRef GRoom;