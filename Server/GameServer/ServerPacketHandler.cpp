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

extern LobbyInformation G_LobbyInfo;

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
			std::string str = pkt.membership_id();
			player->player_id = str;
			player->membership_type = loginPkt.membership_type();

			//cout << *player->player_id << endl;

			// �κ� ����
			GLobby->EnterLobbyLocked(player);
			//GLobby->RoomSend(nullptr, player->player_id);


			//test
			//Protocol::S_ENTER_GAME enterGamePkt;
			//SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
			//GSessionManager.Broadcast(sendBuffer);
			

		}
	}
	file.close(); // ������ �ݽ��ϴ�.
	loginPkt.set_success(isThere);
	SEND_PACKET(loginPkt);
	
	// �κ� ������ Ŭ���̾�Ʈ�� �߰�
	//Lobby->HandleEnterPlayerLocked(static_pointer_cast<GameSession>(session));

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
	std::string str = pkt.membership_id();
	player->player_id = str;
	player->membership_type = membershipPkt.membership_type();
	

	// �κ� ����
	GLobby->EnterLobbyLocked(player);

	return true;
}

bool Handle_C_CREATE_ROOM(PacketSessionRef& session, Protocol::C_CREATE_ROOM& pkt)
{
	// �� ����
	//cout << "Create Room:" << pkt.membership_id() << ", ip:" << pkt.ip_address() << endl;

	// �� ����
	if (pkt.school_type() == Protocol::SCHOOL_TYPE_GREEN) {
		RoomList.emplace_back(pkt.ip_address(), 0, 0, pkt.membership_id(), pkt.membership_type());
		cout << "Create Room GREEN, " << pkt.membership_id() << ", ip:" << pkt.ip_address() << endl;
		cout << "Enter Room GREEN" << pkt.membership_id() << endl;
	}
	else if (pkt.school_type() == Protocol::SCHOOL_TYPE_PURPLE) {
		RoomList.emplace_back(pkt.ip_address(), 0, 0, pkt.membership_id(), pkt.membership_type(), 0);
		cout << "Create Room PURPLE" << pkt.membership_id() << ", ip:" << pkt.ip_address() << endl;
		cout << "Enter Room PURPLE" << pkt.membership_id() << endl;
	}
	else if (pkt.school_type() == Protocol::SCHOOL_TYPE_ORANGE) {
		RoomList.emplace_back(pkt.ip_address(), 0, 0, pkt.membership_id(), pkt.membership_type(), 0, 0);
		cout << "Create Room ORANGE" << pkt.membership_id() << ", ip:" << pkt.ip_address() << endl;
		cout << "Enter Room ORANGE" << pkt.membership_id() << endl;
	}

	return true;
}

bool Handle_C_ENTER_ROOM(PacketSessionRef& session, Protocol::C_ENTER_ROOM& pkt)
{
	switch (pkt.school_type()) {
	case Protocol::SCHOOL_TYPE_GREEN:
		// RoomList �� Index_G�� -1�� ���� ã�Ƽ� �ش� �濡 ����
		for (int i = 0; i < RoomList.size(); i++) {
			if (RoomList[i].Index_G == -1) {
				RoomList[i].Index_G = 0;
				RoomList[i].membership_id_G = pkt.membership_id();
				RoomList[i].membership_type_G = pkt.membership_type();
				RoomList[i].memberCount += 1;
				cout << "Enter Room GREEN" << pkt.membership_id() << endl;
				break;
			}
		}
		break;

	case Protocol::SCHOOL_TYPE_PURPLE:
		// RoomList �� Index_P�� -1�� ���� ã�Ƽ� �ش� �濡 ����
		for (int i = 0; i < RoomList.size(); i++) {
			if (RoomList[i].Index_P == -1) {
				RoomList[i].Index_P = 0;
				RoomList[i].membership_id_P = pkt.membership_id();
				RoomList[i].membership_type_P = pkt.membership_type();
				RoomList[i].memberCount += 1;
				cout << "Enter Room PURPLE" << pkt.membership_id() << endl;
				break;
			}
		}
		break;

	case Protocol::SCHOOL_TYPE_ORANGE:
		// RoomList �� Index_O�� -1�� ���� ã�Ƽ� �ش� �濡 ����
		for (int i = 0; i < RoomList.size(); i++) {
			if (RoomList[i].Index_O == -1) {
				RoomList[i].Index_O = 0;
				RoomList[i].membership_id_O = pkt.membership_id();
				RoomList[i].membership_type_O = pkt.membership_type();
				RoomList[i].memberCount += 1;
				cout << "Enter Room ORANGE" << pkt.membership_id() << endl;
				break;
			}
		}
		break;
	}

	// RoomList �� memberCount�� 2�� ���� ã�Ƽ� ���� ��� Ŭ���̾�Ʈ���� ���� ���� ��Ŷ ����
	for (int i = 0; i < RoomList.size(); i++) {
		if (RoomList[i].memberCount == 1) {
			// ���� ���� ��Ŷ ����
			Protocol::S_ENTER_GAME enterGamePkt;
			enterGamePkt.set_success(true);
			enterGamePkt.set_membership_id_0(RoomList[i].membership_id_G);
			enterGamePkt.set_membership_type_0(RoomList[i].membership_type_G);
			enterGamePkt.set_membership_id_1(RoomList[i].membership_id_P);
			enterGamePkt.set_membership_type_1(RoomList[i].membership_type_P);
			enterGamePkt.set_membership_id_2(RoomList[i].membership_id_O);
			enterGamePkt.set_membership_type_2(RoomList[i].membership_type_O);
			enterGamePkt.set_server_ip(RoomList[i].ip_address);
			//enterGamePkt.set_server_ip("123456789");

			//SEND_PACKET(enterGamePkt);

			{
				// �뿡 �ִ� Ŭ���̾�Ʈ���� ���� ���� ��Ŷ ����
				SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
				GSessionManager.RoomBroadcast(sendBuffer, RoomList[i].membership_id_G);
				GSessionManager.RoomBroadcast(sendBuffer, RoomList[i].membership_id_P);
				GSessionManager.RoomBroadcast(sendBuffer, RoomList[i].membership_id_O);

				cout << enterGamePkt.server_ip() << endl;
			}


			// ���߿� ���� ���� ��Ŷ ������ RoomList���� �ش� �� ����
			//RoomList.erase(RoomList.begin() + i);
			break;
		}
	}	

	return true;
}

bool Handle_C_GAME_OVER(PacketSessionRef& session, Protocol::C_GAME_OVER& pkt)
{

	return true;
}

bool Handle_C_LEAVE_GAME(PacketSessionRef& session, Protocol::C_LEAVE_GAME& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	LobbyRef lobby = player->lobby.load().lock();
	if (lobby == nullptr)
		return false;

	lobby->LeaveLobbyLocked(player);

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

	//SEND_PACKET(privateChatPkt);		// ������ �ϳ��� �ҰŸ� �̰ɷ�

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
