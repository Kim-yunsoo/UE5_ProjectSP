// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Potion/SPPotionBase.h"
#include "SPOrangePotion.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API ASPOrangePotion : public ASPPotionBase
{
	GENERATED_BODY()

		
public:
	ASPOrangePotion();
protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USPOrangeExplosionComponent> OrangeExplosionComponent;

	UFUNCTION()
	void HandleActorHit(AActor* SelfActor,
		AActor* OtherActor,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

	
};
