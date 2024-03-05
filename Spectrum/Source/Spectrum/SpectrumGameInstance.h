// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Spectrum.h"
#include "SpectrumGameInstance.generated.h"

class ATestPlayer;
/**
 * 
 */
UCLASS()
class SPECTRUM_API USpectrumGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// ���� ���� �Լ�, ���� ���� �Լ�
	UFUNCTION(BlueprintCallable)
	void ConnectToGameServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectToGameServer();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	void SendPacket(SendBufferRef SendBuffer);


public:
	void HandleSpawn(const Protocol::PlayerInfo& PlayerInfo, bool IsMine);			// �÷��̾� ������ ����	
	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt);		// ���� ��Ʈ��
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);				// ���� ��Ʈ��

	void HandleDespawn(uint64 ObjectId);								// ������Ʈ ���̵�� ����
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);			// ��Ŷ���� ����

	void HandleMove(const Protocol::S_MOVE& MovePkt);					// �̵�


public:
	// ���� ����, ip
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;
	TSharedPtr<class PacketSession> GameServerSession;	// ���� ���� ����


public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATestPlayer> OtherPlayerClass;

	ATestPlayer* MyPlayer;
	TMap<uint64, ATestPlayer*> Players;
};
