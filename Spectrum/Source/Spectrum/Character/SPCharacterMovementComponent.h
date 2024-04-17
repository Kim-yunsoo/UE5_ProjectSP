// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SPCharacterMovementComponent.generated.h"

/**
 * 
 */


UCLASS()
class SPECTRUM_API USPCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
protected:
	// virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	
protected:
	virtual void MoveAutonomous( float ClientTimeStamp, float DeltaTime, uint8 CompressedFlags, const FVector& NewAccel) override;
};
