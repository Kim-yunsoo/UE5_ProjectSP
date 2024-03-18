// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SPAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/SPCharacterPlayer.h"

USPAnimInstance::USPAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
	bIsAiming = false;
}

void USPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	//GetOwningActor() 현재 소유중인 액터정보를 얻을 수 있다. 리턴 타입이 액터라 캐릭터로 형변환
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
	/*	FProperty* NameProp = ASPCharacterPlayer::StaticClass()->FindPropertyByName(TEXT("bIsAiming"));
		if (NameProp)
		{
			NameProp->GetValue_InContainer(Owner,&bIsAiming);
		}*/
		//bIsAiming= Owner->bis
		/*if (bIsAiming)
			UE_LOG(LogTemp, Log, TEXT("%d"), bIsAiming);*/
		bIsAiming = Owner->bIsAiming;

	}
}
