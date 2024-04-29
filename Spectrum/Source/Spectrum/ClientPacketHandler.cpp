#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "Spectrum.h"
#include "SpectrumGameInstance.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "Protocol.pb.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

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

	// 나중에 로그인 DB에 없으면 회원가입되도록 수정
	if (pkt.success() == true)
	{
		// 로비로 이동
	}
	else
	{
		// 회원가입 창으로 이동
	}

	Protocol::C_ENTER_GAME EnterGamePkt;
	EnterGamePkt.set_playerindex(0);
	SEND_PACKET(EnterGamePkt);


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
