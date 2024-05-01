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
	// DB에서 정보확인 후 있으면 가져오고 클라에 정보 보내기
	// 없으면 생성하라는 패킷 보내기
	bool isThere = false;

	Protocol::S_LOGIN loginPkt;

	cout << "Login : " << pkt.membership_id() << endl;


	std::ifstream file("DB.txt"); // 파일을 엽니다.
	std::string line;

	if (!file.is_open()) {
		std::cerr << "파일을 열 수 없습니다!" << std::endl;
		return -1;
	}else
		std::cout << "파일을 열었습니다." << std::endl;

	while (getline(file, line)) {
		std::istringstream iss(line);
		std::string firstWord, secondWord;
		

		getline(iss, firstWord, ','); // 첫 번째 단어를 읽음
		if (firstWord == pkt.membership_id()) { // 첫 번째 단어가 받은 아이디와 같으면
			if (getline(iss, secondWord, ',')) { // 두 번째 단어를 읽은 후
				std::cout << secondWord << std::endl; // 있으면 두 번째 단어를 출력

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
	file.close(); // 파일을 닫습니다.
	loginPkt.set_success(isThere);
	SEND_PACKET(loginPkt);

	// 로비에 접속한 클라이언트를 추가
	//Lobby->HandleEnterPlayerLocked(static_pointer_cast<GameSession>(session));



	isThere = false;

	return true;
}

bool Handle_C_MEMBERSHIP(PacketSessionRef& session, Protocol::C_MEMBERSHIP& pkt)
{
	cout << pkt.membership_id() << "," << pkt.membership_type() << std::endl;

	bool found = false;

	std::ifstream file("DB.txt"); // 읽기모드
	std::string line;

	if (!file.is_open()) {
		std::cerr << "파일을 열 수 없습니다!" << std::endl;
		return -1;
	}

	// id 있는지 확인
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

	// id가 파일에 없는 경우, 파일 끝에 membership_id, membership_type추가
	if (!found) {
		std::ofstream outFile("DB.txt", std::ios::app); // 쓰기 모드로 파일을 엽니다. append 모드.
		if (!outFile.is_open()) {
			std::cerr << "fail open file" << std::endl;
			return -1;
		}
		outFile << pkt.membership_id() << "," << pkt.membership_type() << std::endl; // "id,state"를 파일 끝에 추가합니다.
		outFile.close(); // 쓰기 작업을 마친 후 파일을 닫습니다.
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
	// 플레이어 생성
	PlayerRef player = ObjectUtils::CreatePlayer(static_pointer_cast<GameSession>(session));

	// 방에 입장
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
