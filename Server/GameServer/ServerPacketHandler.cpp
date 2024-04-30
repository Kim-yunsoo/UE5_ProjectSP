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


	Protocol::S_LOGIN membershipPkt;
	membershipPkt.set_success(true);
	membershipPkt.set_membership_id(pkt.membership_id());
	membershipPkt.set_membership_type(pkt.membership_type());
	SEND_PACKET(membershipPkt);



	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	// �÷��̾� ����
	PlayerRef player = ObjectUtils::CreatePlayer(static_pointer_cast<GameSession>(session));

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
