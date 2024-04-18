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
	virtual void NativeInitializeAnimation() override; //�ִԱ׷��� ���۽� �۵�

	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //�����Ӹ��� ��� ������Ʈ 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ASPCharacterPlayer> Owner; //�ִ� �ν��Ͻ��� �����ϰ� �ִ� ������ ��� ���� 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1; // ����� ��Ȯ�� bool Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould; //�����̴��� 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1; //���������ִ���

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1; //�����ϰ��ִ���

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould; //���������� 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsAiming : 1; //���Ӹ��

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsHolding : 1; //���𰡸� ����ִ���
	
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
