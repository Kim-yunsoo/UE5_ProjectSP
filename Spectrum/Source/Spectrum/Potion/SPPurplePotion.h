// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Potion/SPPotionBase.h"
#include "SPPurplePotion.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API ASPPurplePotion : public ASPPotionBase
{
	GENERATED_BODY()
public:
	ASPPurplePotion();
protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USPPurpleExplosionComponent> PurpleExplosionComponent;

	UFUNCTION()
	void HandleActorHit(AActor* SelfActor,
		AActor* OtherActor,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TObjectPtr<USoundWave> WaterSound;
};
