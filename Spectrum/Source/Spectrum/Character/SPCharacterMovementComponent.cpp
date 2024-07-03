// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPCharacterMovementComponent.h"
//D:\UE_5.3\Engine\Source\Runtime\Engine\Classes\GameFramework\CharacterMovementComponent.h
// #include "GameFramework\CharacterMovementComponent.h"
#include "SpectrumLog.h"
#include "GameFramework/Character.h"

namespace CharacterMovementCVars
{
	// Use newer RPCs and RPC parameter serialization that allow variable length data without changing engine APIs.
	static int32 EnableQueuedAnimEventsOnServer = 1;
	static int32 NetEnableListenServerSmoothing = 1;
	
}


void FSPSavedMove_Character::Clear()
{
	Super::Clear();
	bPressedSlow=false;
	bDidTeleport=false;
}

void FSPSavedMove_Character::SetInitialPosition(ACharacter* Character)
{
	Super::SetInitialPosition(Character);
	USPCharacterMovementComponent* SPMovement = Cast<USPCharacterMovementComponent>(Character->GetCharacterMovement());
	if (SPMovement)
	{
		bPressedSlow = SPMovement->bPressedSlow;
		bDidTeleport = SPMovement->bDidTeleport;
	}

}

uint8 FSPSavedMove_Character::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();
	if (bPressedSlow) //클라이언트 -> 서버로 보낼 때 flag 설정
	{
		Result |= FLAG_Custom_0;
	}

	if (bDidTeleport)
	{
		Result |= FLAG_Custom_1;
	}
	return Result;
}

FSPNetworkPredictionData_Client_Character::FSPNetworkPredictionData_Client_Character(
	const UCharacterMovementComponent& ClientMovement) :Super(ClientMovement)
{
	
}

FSavedMovePtr FSPNetworkPredictionData_Client_Character::AllocateNewMove()
{
	return FSavedMovePtr(new FSPSavedMove_Character());
}

USPCharacterMovementComponent::USPCharacterMovementComponent()
{
	SlowSpeed=100.0f;
	SlowTime=5.0f;
	bPressedSlow=false;
	bDidTeleport=false;
}

void USPCharacterMovementComponent::SetSlowSkillCommand()
{
	bPressedSlow=true;
}

FNetworkPredictionData_Client* USPCharacterMovementComponent::GetPredictionData_Client() const
{
	if (ClientPredictionData == nullptr)
	{
		USPCharacterMovementComponent* MutableThis = const_cast<USPCharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FSPNetworkPredictionData_Client_Character(*this);
	}

	return ClientPredictionData;
}

void USPCharacterMovementComponent::SPSlow()
{
	if(CharacterOwner)
	{

		MaxWalkSpeed=100.f;
		bDidTeleport=true;
	}

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
		{
			bDidTeleport = false;
			MaxWalkSpeed=500.f;
		}
	), SlowTime, false, -1.0f); //반복 없이 일정 시간 이후에 람다 호출 

}

void USPCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	// Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
	if(	bPressedSlow )
	{
		SPSlow();
	}

	if(bPressedSlow)
	{
		bPressedSlow=false;	
	}
	
}

void USPCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
	bPressedSlow = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0; //true라는 뜻
	bDidTeleport = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;

	if (CharacterOwner && CharacterOwner->GetLocalRole() == ROLE_Authority)
	{
		if (bPressedSlow ) //텔레포트가 안된 상황 
		{
			// AB_SUBLOG(LogABTeleport, Log, TEXT("%s"), TEXT("Teleport Begin"));
			SPSlow();
		}
	}
}


void USPCharacterMovementComponent::MoveAutonomous(float ClientTimeStamp, float DeltaTime, uint8 CompressedFlags,
                                                   const FVector& NewAccel)
{
	if (!HasValidData())
	{
		return;
	}

	UpdateFromCompressedFlags(CompressedFlags);
	CharacterOwner->CheckJumpInput(DeltaTime);

	Acceleration = ConstrainInputAcceleration(NewAccel);
	Acceleration = Acceleration.GetClampedToMaxSize(GetMaxAcceleration());
	AnalogInputModifier = ComputeAnalogInputModifier();
	
	const FVector OldLocation = UpdatedComponent->GetComponentLocation();
	const FQuat OldRotation = UpdatedComponent->GetComponentQuat();

	const bool bWasPlayingRootMotion = CharacterOwner->IsPlayingRootMotion();

	PerformMovement(DeltaTime);

	// Check if data is valid as PerformMovement can mark character for pending kill
	if (!HasValidData())
	{
		return;
	}

	// If not playing root motion, tick animations after physics. We do this here to keep events, notifies, states and transitions in sync with client updates.
	if( CharacterOwner && !CharacterOwner->bClientUpdating && !CharacterOwner->IsPlayingRootMotion() && CharacterOwner->GetMesh() )
	{
		if (!bWasPlayingRootMotion) // If we were playing root motion before PerformMovement but aren't anymore, we're on the last frame of anim root motion and have already ticked character
		{
			TickCharacterPose(DeltaTime);
		}
		// TODO: SaveBaseLocation() in case tick moves us?

		if (!CharacterMovementCVars::EnableQueuedAnimEventsOnServer || CharacterOwner->GetMesh()->ShouldOnlyTickMontages(DeltaTime))
		{
			// If we're not doing a full anim graph update on the server, 
			// trigger events right away, as we could be receiving multiple ServerMoves per frame.
			CharacterOwner->GetMesh()->ConditionallyDispatchQueuedAnimEvents();
		}
	}

	if (CharacterOwner && UpdatedComponent)
	{
		// Smooth local view of remote clients on listen servers
		if (CharacterMovementCVars::NetEnableListenServerSmoothing &&
			CharacterOwner->GetRemoteRole() == ROLE_AutonomousProxy &&
			IsNetMode(NM_ListenServer))
		{
			SmoothCorrection(OldLocation, OldRotation, UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentQuat());
		}
	}
	
}
//
// void USPCharacterMovementComponent::Slow()
// {
// 	UE_LOG(LogTemp,Log,TEXT("SLOW!!"));
// 	MaxWalkSpeed = 100.f;
// }
