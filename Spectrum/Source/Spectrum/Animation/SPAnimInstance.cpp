// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SPAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/SPCharacterPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "SpectrumLog.h"

USPAnimInstance::USPAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
	bIsAiming = false;
	bIsHolding = false;
	bIsJumping = false;
	bIsTurnRight = false;
	bIsTurnLeft = false;
	bIsTurnReady = false;	
	DeltaY = 0;
	DeltaZ = 0;
}

void USPAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USPAnimInstance, DeltaY);
	DOREPLIFETIME(USPAnimInstance, DeltaZ);
}

void USPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	//GetOwningActor() 
	Owner = Cast<ASPCharacterPlayer>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void USPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();

		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);

		bIsAiming = Owner->GetIsAiming();
		bIsHolding = Owner->GetIsHolding();

		bIsTurnRight = Owner->bIsTurnRight;
		bIsTurnLeft = Owner->bIsTurnLeft;
		bIsTurnReady= Owner->bIsTurnReady;

		DeltaZ=Owner->DeltaZ;
		DeltaY=Owner->DeltaY;
	}
}

