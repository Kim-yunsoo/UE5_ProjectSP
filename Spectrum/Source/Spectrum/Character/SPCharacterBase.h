// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Protocol.pb.h"
#include "SPCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
class SPECTRUM_API ASPCharacterBase : public ACharacter
{
	GENERATED_BODY()

	public:
	// Sets default values for this character's properties
	ASPCharacterBase();
	virtual ~ASPCharacterBase();


protected:

	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;			// �� �����Ӹ��� ���ŵǾ�� �ϴ� �͵� ���⿡  �Ͷ�����

public:
	bool IsMyPlayer();

	Protocol::MoveState GetMoveState() { return PlayerInfo->state(); }



public:
	void SetMoveState(Protocol::MoveState State);
	void SetPlayerInfo(const Protocol::PlayerInfo& Info);
	void SetDestInfo(const Protocol::PlayerInfo& Info);
	Protocol::PlayerInfo* GetPlayerInfo() { return PlayerInfo; }

	//bool bSendMovePacket;
	//bool GetSendMovePacket() { return bSendMovePacket; }

protected:
	class Protocol::PlayerInfo* PlayerInfo; // ���� ��ġ
	class Protocol::PlayerInfo* DestInfo;	// ������ ��ġ

	void Aiming(const FInputActionValue& Value);
	void StopAiming(const FInputActionValue& Value);
public:
	void Do_Jumping(const FInputActionValue& Value);
	void Stop_Jumping(const FInputActionValue& Value);

protected:
	// ������ȯ
	virtual void SetCharacterControlData(const class USPCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class USPCharacterControlData*> CharacterControlManager;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class USkeletalMeshComponent>Face;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class USkeletalMeshComponent> Torso;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class USkeletalMeshComponent> Legs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class USkeletalMeshComponent> Feet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> Staff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity Gun")
	TObjectPtr<class UPhysicsHandleComponent> PhysicsHandleComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 bIsAiming : 1; //���� 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 bIsHolding : 1; //����� �Ǵ�

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	uint8 bIsJumping : 1; //�����ϰ��ִ���

public:
	const uint8 GetIsAiming() { return bIsAiming; };
	const uint8 GetIsHolding() { return bIsHolding; };
	const uint8 GetIsJumping() { return bIsJumping; };
	
};
