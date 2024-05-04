#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "Spectrum.h"
#include "SpectrumGameInstance.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "UI/SPLobbyWidget.h"
#include "Protocol.pb.h"
//#include <array>
//#include <string>

PacketHandlerFunc GPacketHandler[UINT16_MAX];



extern PlayerInfomation MyPlayerInfo;	// 자신의 정보 저장하는 구조체
extern LobbyInfomation MyLobbyInfo;	// 로비 정보 저장하는 구조체
Protocol::PlayerType school_num_type;
bool okok= false;
int32 NumPlayers;
std::array<Protocol::PlayerType, 3> school_type;/* =
{ Protocol::PLAYER_TYPE_GREEN_MAN ,Protocol::PLAYER_TYPE_PURPLE_WOMAN, Protocol::PLAYER_TYPE_ORANGE_WOMAN };*/

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	return false;
}

bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	//for (auto& Player : pkt.players())
	//{
	//}

	//for (int32 i = 0; i < pkt.players_size(); i++)
	//{
	//	const Protocol::PlayerInfo& Player = pkt.players(i);
	//}

	MyPlayerInfo.issuccess = pkt.success();
	okok= pkt.success();

	//Protocol::C_ENTER_GAME EnterGamePkt;
	//EnterGamePkt.set_playerindex(0);
	//SEND_PACKET(EnterGamePkt);
	
	if (MyPlayerInfo.issuccess) {
		auto myid = pkt.membership_id();
		auto mytype = pkt.membership_type();
		school_num_type = pkt.membership_type();
		FString Fmyid = FString(UTF8_TO_TCHAR(myid.c_str()));
		FString Fmytype;
		MyPlayerInfo.my_membership_id = myid;

		switch (mytype)
		{
		case Protocol::PLAYER_TYPE_GREEN_MAN:
			Fmytype = "PLAYER_TYPE_GREEN_MAN";
			MyPlayerInfo.school_type = 0;
			break;
		case Protocol::PLAYER_TYPE_GREEN_WOMAN:
			Fmytype = "PLAYER_TYPE_GREEN_WOMAN";
			MyPlayerInfo.school_type = 0;
			break;
		case Protocol::PLAYER_TYPE_PURPLE_MAN:
			Fmytype = "PLAYER_TYPE_PURPLE_MAN";
			MyPlayerInfo.school_type = 1;
			break;
		case Protocol::PLAYER_TYPE_PURPLE_WOMAN:
			Fmytype = "PLAYER_TYPE_PURPLE_WOMAN";
			MyPlayerInfo.school_type = 1;
			break;
		case Protocol::PLAYER_TYPE_ORANGE_MAN:
			Fmytype = "PLAYER_TYPE_ORANGE_MAN";
			MyPlayerInfo.school_type = 2;
			break;
		case Protocol::PLAYER_TYPE_ORANGE_WOMAN:
			Fmytype = "PLAYER_TYPE_ORANGE_WOMAN";
			MyPlayerInfo.school_type = 2;
			break;
		}

		NumPlayers = UGameplayStatics::GetPlayerControllerID(GWorld->GetFirstPlayerController());

		// myid,  mytype 출력
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ID:%s, type:%s"), *Fmyid, *Fmytype));
		MyPlayerInfo.player_type = mytype;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Login Failed")));
	}
	return true;
}

bool Handle_S_MEMBERSHIP(PacketSessionRef& session, Protocol::S_MEMBERSHIP& pkt)
{
	if (pkt.success() == true)
	{
		auto myid = pkt.membership_id();
		auto mytype = pkt.membership_type();

		// myi,  mytype 출력
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ID:myid , SCHOOL:mytype")));

		// 자기 회원정보 저장


		// 회원가입 성공 시그널

		
	}

	return true;
}

bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{

	if (auto* GameInstance = Cast<USpectrumGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleLobby(pkt);
	}
	// 나중에 이름까지 구조체에 저장
	school_type[0] = pkt.membership_type_0();
	school_type[1] = pkt.membership_type_1();
	school_type[2] = pkt.membership_type_2();

	return true;
}

bool Handle_S_ENTER_ROOM(PacketSessionRef& session, Protocol::S_ENTER_ROOM& pkt)
{
	if (auto* GameInstance = Cast<USpectrumGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleRoom(pkt);
	}

	return true;
}

bool Handle_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt)
{
	if (auto* GameInstance = Cast<USpectrumGameInstance>(GWorld->GetGameInstance()))
	{
		// 나중에 게임 종료할지 로비로 갈지 결정
	}

	return true;
}

bool Handle_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt)
{
	if (auto* GameInstance = Cast<USpectrumGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSpawn(pkt);
	}

	return true;
}

bool Handle_S_DESPAWN(PacketSessionRef& session, Protocol::S_DESPAWN& pkt)
{
	if (auto* GameInstance = Cast<USpectrumGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleDespawn(pkt);

	}	

	return true;
}

//bool Handle_S_MOVE(PacketSessionRef& session, Protocol::S_MOVE& pkt)
//{
//	if (auto* GameInstance = Cast<USpectrumGameInstance>(GWorld->GetGameInstance()))
//	{
//		GameInstance->HandleMove(pkt);
//	}
//
//	return true;
//}


bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	auto Msg = pkt.msg();
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Msg")));

	return true;
}

bool Handle_S_PUBLIC_CHAT(PacketSessionRef& session, Protocol::S_PUBLIC_CHAT& pkt)
{
	//chat_membership_id = pkt.membership_id();
	//chat_msg = pkt.msg();


	//FString id = FString(UTF8_TO_TCHAR(pkt.membership_id().c_str()));
	//FString chat = FString(UTF8_TO_TCHAR(pkt.msg().c_str()));

	//MyPlayerInfo.mag_id = pkt.membership_id();
	//MyPlayerInfo.mag = pkt.msg();
	//MyPlayerInfo.msg_num = pkt.msg_num();
	// USPLobbyWidget.h의 PublicChatUpdate 함수 호출
	//if (auto* GameInstance = Cast<USpectrumGameInstance>(GWorld->GetGameInstance()))
	//{
	//	if (GameInstance->LobbyWidget != nullptr)
	//	{
	//		GameInstance->LobbyWidget->PublicChatUpdate(id, chat);
	//	}
	//}

	MyLobbyInfo.publicBoard[stoi(pkt.msg_num())].msg_id = pkt.membership_id();
	MyLobbyInfo.publicBoard[stoi(pkt.msg_num())].msg = pkt.msg();
	MyLobbyInfo.publicBoard[stoi(pkt.msg_num())].msg_num = pkt.msg_num();

	return true;
}

bool Handle_S_PRIVATE_CHAT(PacketSessionRef& session, Protocol::S_PRIVATE_CHAT& pkt)
{
	//MyPlayerInfo.pri_mag_id = pkt.membership_id();
	//MyPlayerInfo.pri_mag = pkt.msg();
	//MyPlayerInfo.pri_membership_type = pkt.membership_type();

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

	MyLobbyInfo.privateBoard[school_num].msg_id = pkt.membership_id();
	MyLobbyInfo.privateBoard[school_num].msg = pkt.msg();
	MyLobbyInfo.privateBoard[school_num].membership_type = pkt.membership_type();

	return true;
}