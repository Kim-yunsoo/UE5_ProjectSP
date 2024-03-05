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
	// 서버 연결 함수, 연결 해제 함수
	UFUNCTION(BlueprintCallable)
	void ConnectToGameServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectToGameServer();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	void SendPacket(SendBufferRef SendBuffer);


public:
	void HandleSpawn(const Protocol::PlayerInfo& PlayerInfo, bool IsMine);			// 플레이어 정보로 스폰	
	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt);		// 내가 컨트롤
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);				// 남이 컨트롤

	void HandleDespawn(uint64 ObjectId);								// 오브젝트 아이디로 삭제
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);			// 패킷으로 삭제

	void HandleMove(const Protocol::S_MOVE& MovePkt);					// 이동


public:
	// 서버 소켓, ip
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;
	TSharedPtr<class PacketSession> GameServerSession;	// 게임 서버 만듬


public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATestPlayer> OtherPlayerClass;

	ATestPlayer* MyPlayer;
	TMap<uint64, ATestPlayer*> Players;
};
