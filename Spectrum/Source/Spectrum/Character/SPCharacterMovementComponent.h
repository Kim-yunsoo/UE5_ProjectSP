// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SPCharacterMovementComponent.generated.h"

/**
 * 
 */

class FSPSavedMove_Character : public FSavedMove_Character
{
	typedef FSavedMove_Character Super;
	// 언리얼 오브젝트가 아니기에 super을 사용할 수 없지만 typedef로 Super 키워드 
public:
	virtual void Clear() override;
	virtual void SetInitialPosition(ACharacter* Character) override; //최초 위치, 최초 상태 정보 지정
	virtual uint8 GetCompressedFlags() const override; //클라이언트의 상태 정보를 flag로 넣어서 서버에 보냄

	uint8 bPressedSlow : 1;
	uint8 bDidTeleport : 1;
};

class FSPNetworkPredictionData_Client_Character : public FNetworkPredictionData_Client_Character //클라이언트 캐릭터를 관리할 클레스
{
	typedef FNetworkPredictionData_Client_Character Super;
public:
	FSPNetworkPredictionData_Client_Character(const UCharacterMovementComponent& ClientMovement);
	virtual FSavedMovePtr AllocateNewMove() override; //움직임을 저장하는 구조체를 공유포인터로 반환
};

UCLASS()
class SPECTRUM_API USPCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	USPCharacterMovementComponent();

	void SetSlowSkillCommand();


protected:
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	virtual void SPSlow();
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;


public:
	uint8 bPressedSlow :1;
	uint8 bDidTeleport:1; // 쿨타임이 지날때까지 True상태 

	UPROPERTY()
	float SlowSpeed ;
	
	UPROPERTY()
	float SlowTime;
	// virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	
protected:
	virtual void MoveAutonomous( float ClientTimeStamp, float DeltaTime, uint8 CompressedFlags, const FVector& NewAccel) override;

// public:
// 	void Slow();
};
