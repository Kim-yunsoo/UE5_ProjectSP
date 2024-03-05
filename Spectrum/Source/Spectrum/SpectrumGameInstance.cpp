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
#include "TestMyPlayer.h"


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


void USpectrumGameInstance::HandleSpawn(const Protocol::PlayerInfo& PlayerInfo, bool IsMine)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	// �ߺ� ó�� üũ
	const uint64 ObjectId = PlayerInfo.object_id();
	if (Players.Find(ObjectId) != nullptr)
		return;

	FVector SpawnLocation(PlayerInfo.x(), PlayerInfo.y(), PlayerInfo.z());

	if (IsMine)
	{
		auto* PC = UGameplayStatics::GetPlayerController(this, 0);
		ATestPlayer* Player = Cast<ATestPlayer>(PC->GetPawn());
		if (Player == nullptr)
			return;

		Player->SetPlayerInfo(PlayerInfo);

		MyPlayer = Player;
		Players.Add(PlayerInfo.object_id(), Player);
	}
	else
	{
		ATestPlayer* Player = Cast<ATestPlayer>(World->SpawnActor(OtherPlayerClass, &SpawnLocation));
		Player->SetPlayerInfo(PlayerInfo);
		Players.Add(PlayerInfo.object_id(), Player);
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

	ATestPlayer** FindActor = Players.Find(ObjectId);
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

void USpectrumGameInstance::HandleMove(const Protocol::S_MOVE& MovePkt)
{

	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = MovePkt.info().object_id();
	ATestPlayer** FindActor = Players.Find(ObjectId);
	if (FindActor == nullptr)
		return;

	ATestPlayer* Player = (*FindActor);
	if (Player->IsMyPlayer())
		return;

	const Protocol::PlayerInfo& Info = MovePkt.info();
	//Player->SetPlayerInfo(Info);
	Player->SetDestInfo(Info);

}
