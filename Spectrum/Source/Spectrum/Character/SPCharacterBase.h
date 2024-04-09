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
	virtual void Tick(float DeltaSeconds) override;			

public:
	bool IsMyPlayer();

	Protocol::MoveState GetMoveState() { return PlayerInfo->state(); }

public:
	void SetMoveState(Protocol::MoveState State);
	void SetPostionInfo(const Protocol::PositionInfo& Info);
	void SetDestInfo(const Protocol::PositionInfo& Info);
	Protocol::PositionInfo* GetPlayerInfo() { return PlayerInfo; }

protected:
	bool bIsBlackFour;

protected:
	class Protocol::PositionInfo* PlayerInfo; // 현재 위치
	class Protocol::PositionInfo* DestInfo;	// 가려는 위치


public:
	void AimPotion(const FInputActionValue& Value);
	void ThrowPotion(const FInputActionValue& Value);
	void BlackPotionSpawn(const FInputActionValue& Value);

	void SetBlackFour();

protected:
	
	virtual void SetCharacterControlData(const class USPCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class USPCharacterControlData*> CharacterControlManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Object, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ASPPotionBase> Potion;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 bIsAiming : 1; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 bIsHolding : 1; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 bIsSpawn : 1; //Spawn check

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 bIsThrowReady : 1; //Throw Ready? 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ThrowMontage;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 bIsJumping : 1; 


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
	//turn in place
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurnInPlace", Meta = (AllowPrivateAccess = "true"))
	uint8 bIsTurnRight : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurnInPlace", Meta = (AllowPrivateAccess = "true"))
	uint8 bIsTurnLeft : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurnInPlace", Meta = (AllowPrivateAccess = "true"))
	uint8 bIsTurnReady : 1; // TurnReady?

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurnInPlace", Meta = (AllowPrivateAccess = "true"))
	float PreControlYawRotation;

public:
	const uint8 GetIsAiming() { return bIsAiming; };
	const uint8 GetIsHolding() { return bIsHolding; };
	const uint8 GetIsJumping() { return bIsJumping; };
	const uint8 GetIsThrowReady() { return bIsThrowReady; };
	const uint8 GetIsSpawn() { return bIsSpawn; };

	void SetIsThrowReady(bool throwready) { bIsThrowReady = throwready; };
	void SetIsSpawn(bool spawn) { bIsSpawn = spawn; };

	void SetJumping();
	void ResetJumping();
};
