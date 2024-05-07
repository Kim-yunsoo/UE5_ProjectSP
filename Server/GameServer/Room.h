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

class RoomInformation
{
public:
	// ȣ��Ʈ ip
	std::string ip_address = "-1";

	// �� �ο��� ����
	int memberCount = -1;

	// �濡 ���� ���� ����
	int Index_G = -1;				
	int Index_P = -1;
	int Index_O = -1;

	// �濡 ���� Ŭ���̾�Ʈ ���� ����	
	string membership_id_G = "-1";
	string membership_id_P = "-1";
	string membership_id_O = "-1";
	Protocol::PlayerType membership_type_G = Protocol::PlayerType::PLAYER_TYPE_NONE;
	Protocol::PlayerType membership_type_P = Protocol::PlayerType::PLAYER_TYPE_NONE;
	Protocol::PlayerType membership_type_O = Protocol::PlayerType::PLAYER_TYPE_NONE;

	// �б����� �ٸ� �Ű������� �����ϱ� ���� �б��� �Ű�����
	int school_c = -1;
	int school_m = -1;

	// room.cpp ���� ���� ����� h���� extern���� ����
	// std::vector<RoomInfomation> roomList;
	// ServerPacketHandler���� Handle_C_CREATE_ROOM ������ roomList�� �߰�

	RoomInformation() = default;
	// �׸��� ������ �� �ִ� ������
	explicit RoomInformation(std::string ip_address, int memberCount, int Index_G, string membership_id_G, Protocol::PlayerType membership_type_G)
		: ip_address(ip_address), memberCount(memberCount), Index_G(Index_G), membership_id_G(membership_id_G), membership_type_G(membership_type_G) {}

	// ���ø� ������ �� �ִ� ������
	explicit RoomInformation(std::string ip_address, int memberCount, int Index_P, string membership_id_P, Protocol::PlayerType membership_type_P, int school_c)
		: ip_address(ip_address), memberCount(memberCount), Index_P(Index_P), membership_id_P(membership_id_P), membership_type_P(membership_type_P), school_c(school_c) {}

	// �������� ������ �� �ִ� ������
	explicit RoomInformation(std::string ip_address, int memberCount, int Index_P, string membership_id_P, Protocol::PlayerType membership_type_P, int school_c, int school_m)
		: ip_address(ip_address), memberCount(memberCount), Index_P(Index_P), membership_id_P(membership_id_P), membership_type_P(membership_type_P), school_c(school_c), school_m(school_m) {}
};

class LobbyInformation
{
public:
	// �ۺ� �Խ��� ����
	array <publicchatInfo, 8> publicBoard;

	// �����̺� �Խ��� ����
	array <privatechatInfo, 3> privateBoard;

	// ��Ī�� ����

};


class Lobby : public enable_shared_from_this<Lobby>
{
public:
	Lobby();
	virtual ~Lobby();

public:
	//bool EnterLobby(ObjectRef object, bool randPos = true);
	//bool LeaveLobby(ObjectRef object);
	bool EnterLobbyLocked(PlayerRef player);
	bool LeaveLobbyLocked(PlayerRef player);

private:
	bool EnterLobby(ObjectRef object);
	bool LeaveLobby(uint64 objectId);

	USE_LOCK;

public:
	void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);
	void ObjectBroadcast(SendBufferRef sendBuffer);
	void RoomSend(SendBufferRef sendBuffer, const string* membership_id);

//private:
public:
	unordered_map<uint64, ObjectRef> _player;
	unordered_map<uint64, ObjectRef> _objects;
};

extern LobbyRef GLobby;
extern vector<RoomInformation> RoomList;
//extern unordered_map<uint64, ObjectRef> _player;