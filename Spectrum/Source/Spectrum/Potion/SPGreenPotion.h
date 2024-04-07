// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/SPExplosionComponent.h"
#include "Potion/SPPotionBase.h"
#include "SPGreenPotion.generated.h"

/**
 *
 */
UCLASS()
class SPECTRUM_API ASPGreenPotion : public ASPPotionBase
{
	GENERATED_BODY()
	
	
public:
	ASPGreenPotion();
protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USPExplosionComponent> ExplosionComponent;

	UFUNCTION()
	void HandleActorHit(AActor* SelfActor,
		AActor* OtherActor,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

};
