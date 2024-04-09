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

		// 로비에서 캐릭터 선택창 띄우기
		{ // 로그인	패킷 보내기
			Protocol::C_LOGIN Pkt;
			SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);
			SendPacket(SendBuffer);
			
		}

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("connection failed")));
	}

	//static ConstructorHelpers::FClassFinder<ASPCharacterBase> 
	//	ActorBPClass(TEXT("/Script/Engine.Blueprint'/Game/Spectrum/BluePrint/BP_SPCharacterOther.BP_SPCharacterOther'"));
	//if (ActorBPClass.Succeeded())
	//{
	//	// 찾은 클래스를 멤버 변수에 할당
	//	OtherPlayerClass = ActorBPClass.Class;
	//}

	//// ObjectsClass에 블루프린트 클래스 할당하기
	//static ConstructorHelpers::FClassFinder<ASPObject> 
	//	ObjectClassFinder(TEXT("/Script/Engine.Blueprint'/Game/Gamemode/Test.Test'"));
	//if (ObjectClassFinder.Succeeded())
	//{
	//	ObjectsClass = ObjectClassFinder.Class;
	//}

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


void USpectrumGameInstance::HandleSpawn(const Protocol::ObjectInfo& ObjectInfo, bool IsMine)
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

	FVector SpawnLocation(ObjectInfo.pos_info().x(), ObjectInfo.pos_info().y(), ObjectInfo.pos_info().z());

	if (IsMine)
	{
		auto* PC = UGameplayStatics::GetPlayerController(this, 0);
		ASPCharacterBase* Player = Cast<ASPCharacterBase>(PC->GetPawn());
		if (Player == nullptr)
			return;

		Player->SetPostionInfo(ObjectInfo.pos_info());

		MyPlayer = Player;
		Players.Add(ObjectInfo.object_id(), Player);
	}
	else
	{
		ASPCharacterBase* Player = Cast<ASPCharacterBase>(World->SpawnActor(OtherPlayerClass, &SpawnLocation));
		Player->SetPostionInfo(ObjectInfo.pos_info());
		Players.Add(ObjectInfo.object_id(), Player);
	}

}

void USpectrumGameInstance::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	HandleSpawn(EnterGamePkt.player(), true);
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

	ASPCharacterBase** FindActor = Players.Find(ObjectId);
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

void USpectrumGameInstance::HandleOSpawn(const Protocol::S_O_SPAWN& OSpawnPkt)
{
	auto& Object = OSpawnPkt.objects();

	HandleOSpawn(Object, false);
}

void USpectrumGameInstance::HandleOSpawn(const Protocol::PositionInfo& positionInfo, bool IsMine)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	// 중복 처리 체크
	const uint64 ObjectId = positionInfo.object_id();
	if (Objects.Find(ObjectId) != nullptr)
		return;

	FVector SpawnLocation(positionInfo.x(), positionInfo.y(), positionInfo.z());

	ASPObject* Object = Cast<ASPObject>(World->SpawnActor(ObjectsClass, &SpawnLocation));
	Object->SetPostionInfo(positionInfo);
	Objects.Add(positionInfo.object_id(), Object);

	if (Object)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ok")));

	}
	
}

void USpectrumGameInstance::HandleMove(const Protocol::S_MOVE& MovePkt)
{

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = MovePkt.info().object_id();
	ASPCharacterBase** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	ASPCharacterBase* Player = (*FindActor);
	if (Player->IsMyPlayer())
		return;

	const Protocol::PositionInfo& Info = MovePkt.info();
	//Player->SetPlayerInfo(Info);
	Player->SetDestInfo(Info);

}

void USpectrumGameInstance::HandleOMove(const Protocol::S_O_MOVE& MovePkt)
{

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = MovePkt.info().object_id();
	ASPObject** FindActor = Objects.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	ASPObject* Object = (*FindActor);

	const Protocol::PositionInfo& Info = MovePkt.info();
	//Object->SetPostionInfo(Info);
	Object->SetDestInfo(Info);		// 이동 보정때문에 DestInfo에 저장
}

void USpectrumGameInstance::HandleOBurst(const Protocol::S_O_BURST& BurstPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = BurstPkt.info().object_id();
	ASPObject** FindActor = Objects.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	ASPObject* Object = (*FindActor);

	const Protocol::BurstInfo& Info = BurstPkt.info();
	Object->SetBurst(Info.is_burst());

}
