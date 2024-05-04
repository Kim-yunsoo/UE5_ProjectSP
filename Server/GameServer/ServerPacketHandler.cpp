#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "ObjectUtils.h"
#include "Room.h"
#include "GameSession.h"
#include "Player.h"
#include "Thing.h"
#include <fstream>
#include <sstream>
#include "GameSessionManager.h"

extern LobbyInfomation G_LobbyInfo;

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	return false;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	// DB���� ����Ȯ�� �� ������ �������� Ŭ�� ���� ������
	// ������ �����϶�� ��Ŷ ������
	bool isThere = false;

	Protocol::S_LOGIN loginPkt;

	cout << "Login : " << pkt.membership_id() << endl;


	std::ifstream file("DB.txt"); // ������ ���ϴ�.
	std::string line;

	if (!file.is_open()) {
		std::cerr << "������ �� �� �����ϴ�!" << std::endl;
		return -1;
	}else
		std::cout << "������ �������ϴ�." << std::endl;

	while (getline(file, line)) {
		std::istringstream iss(line);
		std::string firstWord, secondWord;
		

		getline(iss, firstWord, ','); // ù ��° �ܾ ����
		if (firstWord == pkt.membership_id()) { // ù ��° �ܾ ���� ���̵�� ������
			if (getline(iss, secondWord, ',')) { // �� ��° �ܾ ���� ��
				std::cout << secondWord << std::endl; // ������ �� ��° �ܾ ���

				/*PLAYER_TYPE_GREEN_MAN = 00;
				PLAYER_TYPE_GREEN_WOMAN = 01;
				PLAYER_TYPE_PURPLE_MAN = 10;
				PLAYER_TYPE_PURPLE_WOMAN = 11;
				PLAYER_TYPE_ORANGE_MAN = 20;
				PLAYER_TYPE_ORANGE_WOMAN = 21;*/
				
				switch (std::stoi(secondWord))
				{
				case 00:
					loginPkt.set_membership_type(Protocol::PLAYER_TYPE_GREEN_MAN);
					break;
				case 01:
					loginPkt.set_membership_type(Protocol::PLAYER_TYPE_GREEN_WOMAN);
					break;
				case 10:
					loginPkt.set_membership_type(Protocol::PLAYER_TYPE_PURPLE_MAN);
					break;
				case 11:
					loginPkt.set_membership_type(Protocol::PLAYER_TYPE_PURPLE_WOMAN);
					break;
				case 20:
					loginPkt.set_membership_type(Protocol::PLAYER_TYPE_ORANGE_MAN);
					break;
				case 21:
					loginPkt.set_membership_type(Protocol::PLAYER_TYPE_ORANGE_WOMAN);
					break;
				}
				loginPkt.set_membership_id(pkt.membership_id());
			}
			isThere = true;
			// �÷��̾� ����
			PlayerRef player = ObjectUtils::CreatePlayer(static_pointer_cast<GameSession>(session));
			//player->objectInfo->set_player_id(loginPkt.membership_id());
			//player->objectInfo->set_membership_type(loginPkt.membership_type());
			player->player_id = pkt.membership_id();
			player->membership_type = loginPkt.membership_type();

			// �濡 ����
			GRoom->HandleEnterPlayerLocked(player);
		}
	}
	file.close(); // ������ �ݽ��ϴ�.
	loginPkt.set_success(isThere);
	SEND_PACKET(loginPkt);

	// �κ� ������ Ŭ���̾�Ʈ�� �߰�
	//Lobby->HandleEnterPlayerLocked(static_pointer_cast<GameSession>(session));

	//// ��ü �Խ��� ���� ����
	//Protocol::S_PUBLIC_CHAT publicChatPkt;
	//for (int i = 0; i < 8; i++) {
	//	publicChatPkt.set_membership_id(G_LobbyInfo.publicBoard[i].membership_id);
	//	publicChatPkt.set_msg(G_LobbyInfo.publicBoard[i].msg);
	//	publicChatPkt.set_msg_num(G_LobbyInfo.publicBoard[i].msg_num);
	//	SEND_PACKET(publicChatPkt);
	//}

	//// �б� �Խ��� ���� ����
	//Protocol::S_PRIVATE_CHAT privateChatPkt;
	//for (int i = 0; i < 3; i++) {
	//	privateChatPkt.set_membership_id(G_LobbyInfo.privateBoard[i].membership_id);
	//	privateChatPkt.set_msg(G_LobbyInfo.privateBoard[i].msg);
	//	privateChatPkt.set_membership_type(G_LobbyInfo.privateBoard[i].membership_type);
	//	SEND_PACKET(privateChatPkt);
	//}



	isThere = false;

	return true;
}

bool Handle_C_MEMBERSHIP(PacketSessionRef& session, Protocol::C_MEMBERSHIP& pkt)
{
	cout << pkt.membership_id() << "," << pkt.membership_type() << std::endl;

	bool found = false;

	std::ifstream file("DB.txt"); // �б���
	std::string line;

	if (!file.is_open()) {
		std::cerr << "������ �� �� �����ϴ�!" << std::endl;
		return -1;
	}

	// id �ִ��� Ȯ��
	while (getline(file, line)) {
		std::istringstream iss(line);
		std::string firstWord;
		iss >> firstWord;
		if (firstWord == pkt.membership_id()) {
			found = true;
			break;
		}
	}

	file.close();

	// id�� ���Ͽ� ���� ���, ���� ���� membership_id, membership_type�߰�
	if (!found) {
		std::ofstream outFile("DB.txt", std::ios::app); // ���� ���� ������ ���ϴ�. append ���.
		if (!outFile.is_open()) {
			std::cerr << "fail open file" << std::endl;
			return -1;
		}
		outFile << pkt.membership_id() << "," << pkt.membership_type() << std::endl; // "id,state"�� ���� ���� �߰��մϴ�.
		outFile.close(); // ���� �۾��� ��ģ �� ������ �ݽ��ϴ�.
	}

	// Ŭ���̾�Ʈ���� ȸ������ ���� ��Ŷ ����
	Protocol::S_LOGIN membershipPkt;
	membershipPkt.set_success(true);
	membershipPkt.set_membership_id(pkt.membership_id());
	membershipPkt.set_membership_type(pkt.membership_type());
	SEND_PACKET(membershipPkt);

	// �÷��̾� ����
	PlayerRef player = ObjectUtils::CreatePlayer(static_pointer_cast<GameSession>(session));
	//player->objectInfo->set_player_id(loginPkt.membership_id());
	//player->objectInfo->set_membership_type(loginPkt.membership_type());
	player->player_id = pkt.membership_id();
	player->membership_type = membershipPkt.membership_type();

	// �濡 ����
	GRoom->HandleEnterPlayerLocked(player);

	//// ��ü �Խ��� ���� ����
	//Protocol::S_PUBLIC_CHAT publicChatPkt;
	//for (int i = 0; i < 8; i++) {
	//	publicChatPkt.set_membership_id(G_LobbyInfo.publicBoard[i].membership_id);
	//	publicChatPkt.set_msg(G_LobbyInfo.publicBoard[i].msg);
	//	publicChatPkt.set_msg_num(G_LobbyInfo.publicBoard[i].msg_num);
	//	SEND_PACKET(publicChatPkt);
	//}

	//// �б� �Խ��� ���� ����
	//Protocol::S_PRIVATE_CHAT privateChatPkt;
	//for (int i = 0; i < 3; i++) {
	//	privateChatPkt.set_membership_id(G_LobbyInfo.privateBoard[i].membership_id);
	//	privateChatPkt.set_msg(G_LobbyInfo.privateBoard[i].msg);
	//	privateChatPkt.set_membership_type(G_LobbyInfo.privateBoard[i].membership_type);
	//	SEND_PACKET(privateChatPkt);
	//}


	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	// �÷��̾� ����
	PlayerRef player = ObjectUtils::CreatePlayer(static_pointer_cast<GameSession>(session));
	//player->objectInfo->set_membership_id(pkt.membership_id());
	//player->objectInfo->set_membership_type(pkt.membership_type());
	
	// �濡 ����
	GRoom->HandleEnterPlayerLocked(player);

	cout<< "Player Enter" << endl;

	return true;
}

bool Handle_C_ENTER_ROOM(PacketSessionRef& session, Protocol::C_ENTER_ROOM& pkt)
{
	Protocol::S_ENTER_ROOM enterRoomPkt;
	Protocol::PlayerInfo* player = enterRoomPkt.add_players();
	enterRoomPkt.set_success(true);
	SEND_PACKET(enterRoomPkt);

	return false;
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

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	return true;
}

bool Handle_C_PUBLIC_CHAT(PacketSessionRef& session, Protocol::C_PUBLIC_CHAT& pkt)
{
	Protocol::S_PUBLIC_CHAT publicChatPkt;

	cout << "Public Chat : " << pkt.membership_id() << " : " << pkt.msg() << endl;

	publicChatPkt.set_membership_id(pkt.membership_id());
	publicChatPkt.set_msg(pkt.msg());
	publicChatPkt.set_msg_num(pkt.msg_num());

	SEND_PACKET(publicChatPkt);

	auto send_Buffer = ServerPacketHandler::MakeSendBuffer(publicChatPkt);
	GSessionManager.Broadcast(send_Buffer);

	/*	string membership_id;
	string msg;
	string msg_num;*/

	G_LobbyInfo.publicBoard[stoi(pkt.msg_num())].membership_id = pkt.membership_id();
	G_LobbyInfo.publicBoard[stoi(pkt.msg_num())].msg = pkt.msg();
	G_LobbyInfo.publicBoard[stoi(pkt.msg_num())].msg_num = pkt.msg_num();

	return true;
}

bool Handle_C_PRIVATE_CHAT(PacketSessionRef& session, Protocol::C_PRIVATE_CHAT& pkt)
{
	// ���̵�� �б� �˻� �� �б��� ���� ��쿡�� ����
	Protocol::S_PRIVATE_CHAT privateChatPkt;

	cout << "Provate Chat : " << pkt.membership_id() << " : " << pkt.msg() << endl;

	privateChatPkt.set_membership_id(pkt.membership_id());
	privateChatPkt.set_msg(pkt.msg());
	privateChatPkt.set_membership_type(pkt.membership_type());

	SEND_PACKET(privateChatPkt);

	auto send_Buffer = ServerPacketHandler::MakeSendBuffer(privateChatPkt);
	GSessionManager.Broadcast(send_Buffer);

	/*	string membership_id;
	string msg;
	Protocol::SchoolType membership_type;*/
	int school_num = -1;
	switch (pkt.membership_type()) {
	case Protocol::SCHOOL_TYPE_GREEN:
		school_num = 0;
		break;

	case Protocol::SCHOOL_TYPE_PURPLE:
		school_num = 1;
		break;

	case Protocol::SCHOOL_TYPE_ORANGE:
		school_num = 2;
		break;
	}
	if (school_num != -1) {
		G_LobbyInfo.privateBoard[school_num].membership_id = pkt.membership_id();
		G_LobbyInfo.privateBoard[school_num].msg = pkt.msg();
		G_LobbyInfo.privateBoard[school_num].membership_type = pkt.membership_type();
	}


	return true;
}
