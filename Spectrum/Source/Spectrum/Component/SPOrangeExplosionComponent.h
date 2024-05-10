// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/SPComponentBase.h"
#include "SPOrangeExplosionComponent.generated.h"

/**
 * 
 */
UCLASS()
class SPECTRUM_API USPOrangeExplosionComponent : public USPComponentBase
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	USPOrangeExplosionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void Explode() override;

	//RPC
	UFUNCTION(NetMulticast, Unreliable)
	void MultiRPCOrangeExplosion(const TArray<FHitResult>& Array);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TObjectPtr<USoundWave> WaterSound;
};
