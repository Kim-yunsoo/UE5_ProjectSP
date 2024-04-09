// Fill out your copyright notice in the Description page of Project Settings.
// ���� ������ ����, ��Ŀ��Ʈ ���⼭ ��.

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
	// ���� ����
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	// ������ ����(��Ʈ��ũ Ŀ�ؼ�)
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("connect to server")));

	bool bConnected = Socket->Connect(*InternetAddr);

	// ���� ���� ����
	if (bConnected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("connection Success")));

		// Session �����(��Ŀ ������ ����)
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run();		// ���� ��Ƽ������� ���ư�

		// �κ񿡼� ĳ���� ����â ����
		{ // �α���	��Ŷ ������
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
	//	// ã�� Ŭ������ ��� ������ �Ҵ�
	//	OtherPlayerClass = ActorBPClass.Class;
	//}

	//// ObjectsClass�� �������Ʈ Ŭ���� �Ҵ��ϱ�
	//static ConstructorHelpers::FClassFinder<ASPObject> 
	//	ObjectClassFinder(TEXT("/Script/Engine.Blueprint'/Game/Gamemode/Test.Test'"));
	//if (ObjectClassFinder.Succeeded())
	//{
	//	ObjectsClass = ObjectClassFinder.Class;
	//}

}

void USpectrumGameInstance::DisconnectToGameServer()
{

	// Ŭ���̾�Ʈ�� �����ϸ� �������� �˷������
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

	GameServerSession->HandleRecvPackets();	// ��Ŀ �����尡 ���� ť�� �Ҹ��Ͽ� ó��

}

void USpectrumGameInstance::SendPacket(SendBufferRef SendBuffer)
{

	if(Socket == nullptr || GameServerSession == nullptr)
	{
		return;
	}

	GameServerSession->SendPacket(SendBuffer);	// ť�� �׾��ִ� ����
}


void USpectrumGameInstance::HandleSpawn(const Protocol::ObjectInfo& ObjectInfo, bool IsMine)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	// �ߺ� ó�� üũ
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

	// �ߺ� ó�� üũ
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
	Object->SetDestInfo(Info);		// �̵� ���������� DestInfo�� ����
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
