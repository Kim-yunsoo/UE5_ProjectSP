#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "Spectrum.h"
#include "SpectrumGameInstance.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "UI/SPLobbyWidget.h"
#include "Protocol.pb.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];
bool issuccess;

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

	issuccess = pkt.success();
	

	//Protocol::C_ENTER_GAME EnterGamePkt;
	//EnterGamePkt.set_playerindex(0);
	//SEND_PACKET(EnterGamePkt);
	
	if (issuccess) {
		auto myid = pkt.membership_id();
		auto mytype = pkt.membership_type();
		FString Fmyid = FString(UTF8_TO_TCHAR(myid.c_str()));
		FString Fmytype;

		switch (mytype)
		{
		case Protocol::PLAYER_TYPE_GREEN_MAN:
			Fmytype = "PLAYER_TYPE_GREEN_MAN";
			break;
		case Protocol::PLAYER_TYPE_GREEN_WOMAN:
			Fmytype = "PLAYER_TYPE_GREEN_WOMAN";
			break;
		case Protocol::PLAYER_TYPE_PURPLE_MAN:
			Fmytype = "PLAYER_TYPE_PURPLE_MAN";
			break;
		case Protocol::PLAYER_TYPE_PURPLE_WOMAN:
			Fmytype = "PLAYER_TYPE_PURPLE_WOMAN";
			break;
		case Protocol::PLAYER_TYPE_ORANGE_MAN:
			Fmytype = "PLAYER_TYPE_ORANGE_MAN";
			break;
		case Protocol::PLAYER_TYPE_ORANGE_WOMAN:
			Fmytype = "PLAYER_TYPE_ORANGE_WOMAN";
			break;
		}

		// myi,  mytype 출력
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ID:%s, type:%s"), *Fmyid, *Fmytype));

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
