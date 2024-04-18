// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SPAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class SPECTRUM_API USPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	public:
	USPAnimInstance();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void NativeInitializeAnimation() override; //애님그래프 시작시 작동

	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //프레임마다 계속 업데이트 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ASPCharacterPlayer> Owner; //애님 인스턴스를 소유하고 있는 정보를 담는 변수 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1; // 사이즈가 명확한 bool 타입

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould; //움직이는지 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1; //떨어지고있는지

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1; //점프하고있는지

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould; //점프중인지 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsAiming : 1; //에임모드

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsHolding : 1; //무언가를 들고있는지
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float DeltaY;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float DeltaZ;
	
	//turn in place
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurnInPlace", Meta = (AllowPrivateAccess = "true"))
	uint8 bIsTurnRight : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurnInPlace", Meta = (AllowPrivateAccess = "true"))
	uint8 bIsTurnLeft : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurnInPlace", Meta = (AllowPrivateAccess = "true"))
	uint8 bIsTurnReady : 1;
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
