// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPCharacterMovementComponent.h"
#include "GameFramework/Character.h"


void USPCharacterMovementComponent::MoveAutonomous(float ClientTimeStamp, float DeltaTime, uint8 CompressedFlags,
                                                   const FVector& NewAccel)
{
	//Super::MoveAutonomous(ClientTimeStamp, DeltaTime, CompressedFlags, NewAccel);
	
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
			//TickCharacterPose(DeltaTime);
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
