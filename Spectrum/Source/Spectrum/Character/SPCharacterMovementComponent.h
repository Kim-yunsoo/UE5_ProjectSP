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
	// �𸮾� ������Ʈ�� �ƴϱ⿡ super�� ����� �� ������ typedef�� Super Ű���� 
public:
	virtual void Clear() override;
	virtual void SetInitialPosition(ACharacter* Character) override; //���� ��ġ, ���� ���� ���� ����
	virtual uint8 GetCompressedFlags() const override; //Ŭ���̾�Ʈ�� ���� ������ flag�� �־ ������ ����

	uint8 bPressedSlow : 1;
	uint8 bDidTeleport : 1;
};

class FSPNetworkPredictionData_Client_Character : public FNetworkPredictionData_Client_Character //Ŭ���̾�Ʈ ĳ���͸� ������ Ŭ����
{
	typedef FNetworkPredictionData_Client_Character Super;
public:
	FSPNetworkPredictionData_Client_Character(const UCharacterMovementComponent& ClientMovement);
	virtual FSavedMovePtr AllocateNewMove() override; //�������� �����ϴ� ����ü�� ���������ͷ� ��ȯ
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
	uint8 bDidTeleport:1; // ��Ÿ���� ���������� True���� 

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
