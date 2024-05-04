// Fill out your copyright notice in the Description page of Project Settings.
// 게임 서버에 접속, 디스커넥트 여기서 함.

#include "SpectrumGameInstance.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "Protocol.pb.h"
#include "ClientPacketHandler.h"
#include "Object/SPObject.h"
#include "Character/SPCharacterPlayer.h"
#include "UI/SPLobbyWidget.h"


void USpectrumGameInstance::ConnectToGameServer()
{
	// 소켓 생성
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	// 서버에 접속(네트워크 커넥션)
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("connect to server")));

	bool bConnected = Socket->Connect(*InternetAddr);

	// 접속 성공 여부
	if (bConnected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("connection Success")));

		// Session 만들기(워커 스레드 생성)
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run();		// 이제 멀티스레드로 돌아감

		//// 로비에서 캐릭터 선택창 띄우기
		//{ // 로그인	패킷 보내기
		//	Protocol::C_LOGIN Pkt;
		//	//SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);

		//	//SendPacket(SendBuffer);
		//	Pkt.set_id("test");

			//SEND_PACKET(Pkt);
		//}

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("connection failed")));
	}

	//LobbyWidget = CreateWidget<USPLobbyWidget>(this, LobbyWidgetClass);
	//PlayerType = 0;

}

void USpectrumGameInstance::DisconnectToGameServer()
{

	// 클라이언트가 종료하면 서버에게 알려줘야함
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	Protocol::C_LEAVE_GAME LeavePkt;
	SEND_PACKET(LeavePkt);

	/*if(Socket)
	{
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
	}*/
}

void USpectrumGameInstance::HandleRecvPackets()
{

	if (Socket == nullptr || GameServerSession == nullptr)
	{
		return;
	}

	GameServerSession->HandleRecvPackets();	// 워커 스레드가 긁은 큐를 소모하여 처리

}

void USpectrumGameInstance::SendPacket(SendBufferRef SendBuffer)
{

	if(Socket == nullptr || GameServerSession == nullptr)
	{
		return;
	}

	GameServerSession->SendPacket(SendBuffer);	// 큐를 쌓아주는 역할
}

//void USpectrumGameInstance::AsyncClientPawn(const FString& serverIP, const int32 ServerPort, int32 PawnIndex)
//{
//	if (Socket == nullptr || GameServerSession == nullptr)
//		return;
//
//	TArray<uint8> DataArray;
//	DataArray.Append(reinterpret_cast<const uint8*>(&PawnIndex), sizeof(int32));
//
//	int32 BytesSent = 0;
//	Socket->Send(DataArray.GetData(), DataArray.Num(), BytesSent);
//}

void USpectrumGameInstance::HandleSpawn(const Protocol::PlayerInfo& ObjectInfo, bool IsMine)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	// 중복 처리 체크
	const uint64 ObjectId = ObjectInfo.object_id();
	if (Players.Find(ObjectId) != nullptr)
		return;

	//FVector SpawnLocation(ObjectInfo.pos_info().x(), ObjectInfo.pos_info().y(), ObjectInfo.pos_info().z());

	if (IsMine)
	{
		auto* PC = UGameplayStatics::GetPlayerController(this, 0);
		ASPCharacterPlayer* Player = Cast<ASPCharacterPlayer>(PC->GetPawn());
		if (Player == nullptr)
			return;

		//Player->SetPostionInfo(ObjectInfo.pos_info());

		MyPlayer = Player;
		Players.Add(ObjectInfo.object_id(), Player);
	}


}

void USpectrumGameInstance::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	//HandleSpawn(EnterGamePkt.player(), true);
}

void USpectrumGameInstance::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
{
	for (auto& Player : SpawnPkt.players())
	{
		HandleSpawn(Player, false);
	}
}

void USpectrumGameInstance::HandleDespawn(uint64 ObjectId)
{

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	ASPCharacterPlayer** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	World->DestroyActor(*FindActor);
}

void USpectrumGameInstance::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{

	for (auto& ObjectId : DespawnPkt.object_ids())
	{
		HandleDespawn(ObjectId);
	}
}

void USpectrumGameInstance::HandleLobby(const Protocol::S_ENTER_GAME& LobbyPkt)
{
	// 받은 정보 저장(자신의 학교, 성별)

}

void USpectrumGameInstance::HandleLobby(const Protocol::S_LOGIN& LobbyPkt)
{
	// 받은 정보 저장(자신의 학교, 성별)
	//const Protocol::PositionInfo& Info = LobbyPkt.info();
	//Player->SetDestInfo(Info);

}

void USpectrumGameInstance::HandleRoom(const Protocol::S_ENTER_ROOM& RoomPkt)
{
	// 방 입장
	// 방 안의 정보 저장(방에 있는 사람들 정보)
}



//void USpectrumGameInstance::HandleMove(const Protocol::S_MOVE& MovePkt)
//{
//
//	if (Socket == nullptr || GameServerSession == nullptr)
//		return;
//
//	auto* World = GetWorld();
//	if (World == nullptr)
//		return;
//
//	const uint64 ObjectId = MovePkt.info().object_id();
//	ASPCharacterPlayer** FindActor = Players.Find(ObjectId);
//	if (FindActor == nullptr)
//		return;
//
//	//ASPCharacterPlayer* Player = (*FindActor);
//	//if (Player->IsMyPlayer())
//	//	return;
//
//	//const Protocol::PositionInfo& Info = MovePkt.info();
//	////Player->SetPlayerInfo(Info);
//	//Player->SetDestInfo(Info);
//
//}
//

