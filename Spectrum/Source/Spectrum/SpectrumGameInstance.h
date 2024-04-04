// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Spectrum.h"
#include "SpectrumGameInstance.generated.h"

class ASPCharacterBase;
class ASPObject;
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
	void HandleSpawn(const Protocol::ObjectInfo& ObjectInfo, bool IsMine);			// �÷��̾� ������ ����	
	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt);		// ���� ��Ʈ��
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);				// ���� ��Ʈ��

	void HandleDespawn(uint64 ObjectId);								// ������Ʈ ���̵�� ����
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);			// ��Ŷ���� ����
	
	
	void HandleOSpawn(const Protocol::S_O_SPAWN& OSpawnPkt);			// ��Ŷ���� ����
	void HandleOSpawn(const Protocol::PositionInfo& positionInfo, bool IsMine);

	void HandleMove(const Protocol::S_MOVE& MovePkt);					// �̵�
	void HandleOMove(const Protocol::S_O_MOVE& MovePkt);					// �̵�


public:
	// ���� ����, ip
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;
	TSharedPtr<class PacketSession> GameServerSession;	// ���� ���� ����


public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASPCharacterBase> OtherPlayerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASPObject> ObjectsClass;

	ASPCharacterBase* MyPlayer;
	TMap<uint64, ASPCharacterBase*> Players;
	TMap<uint64, ASPObject*> Objects;
};
