// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SPAIAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USPAIAnimInstance::USPAIAnimInstance()
{
	
}

void USPAIAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(GetOwningActor());
	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();
	}
}

void USPAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (MovementComponent)
	{
		Velocity = MovementComponent->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsFalling = MovementComponent->IsFalling();
		//Direction ¼³Á¤ 
		FRotator BaseRotation = GetOwningActor()->GetActorRotation();
		Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, BaseRotation);
	}
}
