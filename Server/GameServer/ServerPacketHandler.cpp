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
			// 플레이어 생성
			PlayerRef player = ObjectUtils::CreatePlayer(static_pointer_cast<GameSession>(session));
			//player->objectInfo->set_player_id(loginPkt.membership_id());
			//player->objectInfo->set_membership_type(loginPkt.membership_type());
			player->player_id = pkt.membership_id();
			player->membership_type = loginPkt.membership_type();

			// 방에 입장
			GRoom->HandleEnterPlayerLocked(player);
		}
	}
	file.close(); // 파일을 닫습니다.
	loginPkt.set_success(isThere);
	SEND_PACKET(loginPkt);

	{
		// S_ENTER_GAME 보내기 -> 나중에 매치메이킹 구현하고 고치기
		// 매치메이킹 구현 전 임시로 보냄
		// 서버만들기 버튼 누른 사람이 0번
		// 나머지는 순서대로 1, 2번
		// 룸 정보 어디에 저장할지 생각하기

		Protocol::S_ENTER_GAME enterGamePkt;
		enterGamePkt.set_success(true);
		enterGamePkt.set_membership_id_0("test1");
		enterGamePkt.set_membership_type_0(Protocol::PLAYER_TYPE_GREEN_MAN);
		enterGamePkt.set_membership_id_1("test2");
		enterGamePkt.set_membership_type_1(Protocol::PLAYER_TYPE_PURPLE_WOMAN);
		enterGamePkt.set_membership_id_2("test3");
		enterGamePkt.set_membership_type_2(Protocol::PLAYER_TYPE_ORANGE_WOMAN);

		//enterGamePkt.set_membership_0().set_membership_id("test1");
		//enterGamePkt.set_membership_0().set_membership_type(Protocol::PLAYER_TYPE_GREEN_MAN);
		//enterGamePkt.set_membership_1().set_membership_id("test2");
		//enterGamePkt.set_membership_1().set_membership_type(Protocol::PLAYER_TYPE_PURPLE_WOMAN);
		//enterGamePkt.set_membership_2().set_membership_id("test3");
		//enterGamePkt.set_membership_2().set_membership_type(Protocol::PLAYER_TYPE_ORANGE_WOMAN);

		SEND_PACKET(enterGamePkt);
	}


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

	// 클라이언트에게 회원가입 성공 패킷 전송
	Protocol::S_LOGIN membershipPkt;
	membershipPkt.set_success(true);
	membershipPkt.set_membership_id(pkt.membership_id());
	membershipPkt.set_membership_type(pkt.membership_type());
	SEND_PACKET(membershipPkt);

	// 플레이어 생성
	PlayerRef player = ObjectUtils::CreatePlayer(static_pointer_cast<GameSession>(session));
	//player->objectInfo->set_player_id(loginPkt.membership_id());
	//player->objectInfo->set_membership_type(loginPkt.membership_type());
	player->player_id = pkt.membership_id();
	player->membership_type = membershipPkt.membership_type();

	// 방에 입장
	GRoom->HandleEnterPlayerLocked(player);

	//// 전체 게시판 정보 전송
	//Protocol::S_PUBLIC_CHAT publicChatPkt;
	//for (int i = 0; i < 8; i++) {
	//	publicChatPkt.set_membership_id(G_LobbyInfo.publicBoard[i].membership_id);
	//	publicChatPkt.set_msg(G_LobbyInfo.publicBoard[i].msg);
	//	publicChatPkt.set_msg_num(G_LobbyInfo.publicBoard[i].msg_num);
	//	SEND_PACKET(publicChatPkt);
	//}

	//// 학교 게시판 정보 전송
	//Protocol::S_PRIVATE_CHAT privateChatPkt;
	//for (int i = 0; i < 3; i++) {
	//	privateChatPkt.set_membership_id(G_LobbyInfo.privateBoard[i].membership_id);
	//	privateChatPkt.set_msg(G_LobbyInfo.privateBoard[i].msg);
	//	privateChatPkt.set_membership_type(G_LobbyInfo.privateBoard[i].membership_type);
	//	SEND_PACKET(privateChatPkt);
	//}


	return true;
}

bool Handle_C_CREATE_ROOM(PacketSessionRef& session, Protocol::C_CREATE_ROOM& pkt)
{
	// 방 생성


	return true;
}

//bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
//{
//	// 플레이어 생성
//	PlayerRef player = ObjectUtils::CreatePlayer(static_pointer_cast<GameSession>(session));
//	//player->objectInfo->set_membership_id(pkt.membership_id());
//	//player->objectInfo->set_membership_type(pkt.membership_type());
//	
//	// 방에 입장
//	GRoom->HandleEnterPlayerLocked(player);
//
//	cout<< "Player Enter" << endl;
//
//	return true;
//}

bool Handle_C_ENTER_ROOM(PacketSessionRef& session, Protocol::C_ENTER_ROOM& pkt)
{
	//Protocol::S_ENTER_ROOM enterRoomPkt;
	//Protocol::PlayerInfo* player = enterRoomPkt.add_players();
	//enterRoomPkt.set_success(true);
	//SEND_PACKET(enterRoomPkt);

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

	lobby->HandleLeavePlayerLocked(player);

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
	// 아이디로 학교 검색 후 학교가 같을 경우에만 전송
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
