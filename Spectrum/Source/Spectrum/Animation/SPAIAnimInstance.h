// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SPAIAnimInstance.generated.h"

class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class SPECTRUM_API USPAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	USPAIAnimInstance();
protected:
	virtual void NativeInitializeAnimation() override; //애님그래프 시작시 작동

	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //프레임마다 계속 업데이트

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<ACharacter> Character;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float GroundSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	uint8 bIsFalling :1;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float Direction;
	
};
