//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Character.h"
//#include "InputActionValue.h"
//#include "Protocol.pb.h"
//#include "TestPlayer.generated.h"
//
//UCLASS()
//class ATestPlayer : public ACharacter
//{
//	GENERATED_BODY()
//
//public:
//	ATestPlayer();
//	virtual ~ATestPlayer();
//
//protected:
//
//	virtual void BeginPlay();
//	virtual void Tick(float DeltaSeconds) override;			// �� �����Ӹ��� ���ŵǾ�� �ϴ� �͵� ���⿡  �Ͷ�����
//
//
//public:
//	bool IsMyPlayer();
//
//	Protocol::MoveState GetMoveState() { return PlayerInfo->state(); }
//	void SetMoveState(Protocol::MoveState State);
//
//
//public:
//	void SetPlayerInfo(const Protocol::PlayerInfo& Info);
//	void SetDestInfo(const Protocol::PlayerInfo& Info);
//	Protocol::PlayerInfo* GetPlayerInfo() { return PlayerInfo; }
//
//
//protected:
//	class Protocol::PlayerInfo* PlayerInfo; // ���� ��ġ
//	class Protocol::PlayerInfo* DestInfo;	// ������ ��ġ
//
//
//};
//
