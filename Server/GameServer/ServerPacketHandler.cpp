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
			std::string str = pkt.membership_id();
			player->player_id = str;
			player->membership_type = loginPkt.membership_type();

			//cout << *player->player_id << endl;

			// 로비에 입장
			GLobby->EnterLobbyLocked(player);
			//GLobby->RoomSend(nullptr, player->player_id);


			//test
			//Protocol::S_ENTER_GAME enterGamePkt;
			//SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
			//GSessionManager.Broadcast(sendBuffer);
			

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
	std::string str = pkt.membership_id();
	player->player_id = str;
	player->membership_type = membershipPkt.membership_type();
	

	// 로비에 입장
	GLobby->EnterLobbyLocked(player);

	return true;
}

bool Handle_C_CREATE_ROOM(PacketSessionRef& session, Protocol::C_CREATE_ROOM& pkt)
{
	// 방 생성
	//cout << "Create Room:" << pkt.membership_id() << ", ip:" << pkt.ip_address() << endl;

	// 방 생성
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
		// RoomList 중 Index_G이 -1인 방을 찾아서 해당 방에 입장
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
		// RoomList 중 Index_P이 -1인 방을 찾아서 해당 방에 입장
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
		// RoomList 중 Index_O이 -1인 방을 찾아서 해당 방에 입장
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

	// RoomList 중 memberCount가 2인 방을 찾아서 방의 모든 클라이언트에게 게임 시작 패킷 전송
	for (int i = 0; i < RoomList.size(); i++) {
		if (RoomList[i].memberCount == 2) {
			// 게임 시작 패킷 전송
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
				// 룸에 있는 클라이언트에게 게임 시작 패킷 전송
				SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
				GSessionManager.RoomBroadcast(sendBuffer, RoomList[i].membership_id_G);
				GSessionManager.RoomBroadcast(sendBuffer, RoomList[i].membership_id_P);
				GSessionManager.RoomBroadcast(sendBuffer, RoomList[i].membership_id_O);

				cout << enterGamePkt.server_ip() << endl;
			}


			// 나중에 게임 종료 패킷 받으면 RoomList에서 해당 방 삭제
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
	// 아이디로 학교 검색 후 학교가 같을 경우에만 전송
	Protocol::S_PRIVATE_CHAT privateChatPkt;

	cout << "Provate Chat : " << pkt.membership_id() << " : " << pkt.msg() << endl;

	privateChatPkt.set_membership_id(pkt.membership_id());
	privateChatPkt.set_msg(pkt.msg());
	privateChatPkt.set_membership_type(pkt.membership_type());

	//SEND_PACKET(privateChatPkt);		// 에디터 하나만 할거면 이걸로

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
