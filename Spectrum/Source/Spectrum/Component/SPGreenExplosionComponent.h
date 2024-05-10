// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPComponentBase.h"
#include "Components/ActorComponent.h"
#include "SPGreenExplosionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPECTRUM_API USPGreenExplosionComponent : public USPComponentBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USPGreenExplosionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void Explode() override;

	//RPC
	UFUNCTION(NetMulticast, Unreliable)
	void MultiRPCGreenExplosion(const TArray<FHitResult>& Array);
	
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TObjectPtr<USoundWave> WaterSound;


};
