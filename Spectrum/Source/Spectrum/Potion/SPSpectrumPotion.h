// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Potion/SPPotionBase.h"
#include "SPSpectrumPotion.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API ASPSpectrumPotion : public ASPPotionBase
{
	GENERATED_BODY()

public:
	ASPSpectrumPotion();
protected:
	virtual void BeginPlay() override;

	
	UFUNCTION()
	void HandleActorHit(AActor* SelfActor,
		AActor* OtherActor,
		FVector NormalImpulse,
		const FHitResult& Hit
	);
	
};
